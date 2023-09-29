#pragma once
#include <string>
#include <format>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <ranges>
#include <concepts>

#include <librepr/detail/type_list.h>
#include <librepr/detail/concepts.h>
#include <librepr/repr/repr.h>

#include "to_tuple.h"

namespace librepr {
template <typename T>
struct Reflect {
  using type = T;

  static std::string dump(T const& obj, bool with_type = true, bool /*explicit_types*/ = false) {
    if (with_type) {
      return librepr::get_name<T>() + librepr::repr(obj);
    }
    return librepr::repr(obj);
  }

  static std::string layout() { return librepr::get_name<T>(); }
};

template <typename T>
  requires std::is_fundamental_v<T>
struct Reflect<T> {
  using type = T;

  static std::string dump(T const& obj, bool /*with_type*/ = false, bool /*explicit_types*/ = false) {
    return librepr::repr(obj);
  }

  static std::string layout() { return librepr::get_name<T>(); }
};

template <detail::has_repr_member T>
struct Reflect<T> {
  using type = T;

  static std::string dump(T const& obj, bool /*with_type*/ = true, bool /*explicit_types*/ = false) {
    // TODO pass down explicit_types
    return obj.repr();
  }

  static std::string layout() {
    // TODO check for layout member function
    return librepr::get_name<T>();
  }
};

template <detail::pair_like T>
struct Reflect<T> {
  using type        = T;
  using first_type  = std::remove_cv_t<std::remove_reference_t<typename T::first_type>>;
  using second_type = std::remove_cv_t<std::remove_reference_t<typename T::second_type>>;

  static std::string dump(T const& obj, bool with_type = true, bool explicit_types = false) {
    return std::format("{}{{{}, {}}}", (with_type) ? librepr::get_name<T>() : "",
                       Reflect<first_type>::dump(obj.first, explicit_types, explicit_types),
                       Reflect<second_type>::dump(obj.second, explicit_types, explicit_types));
  }

  static std::string layout() {
    return std::format("{{{}, {}}}", librepr::get_name<typename T::first_type>(),
                       librepr::get_name<typename T::second_type>());
  }
};

template <typename T>
  requires std::is_aggregate_v<T> && (not detail::has_repr_member<T>) && (not std::is_array_v<T>)
struct Reflect<T> {
  using member_tuple = decltype(librepr::detail::to_tuple(std::declval<T>()));
  using type         = librepr::detail::TypeList<>::from_tuple<member_tuple>::template map_t<
      std::remove_reference>::template map_t<std::remove_cv>::template map<librepr::Reflect>;

  static std::string dump(T const& obj, bool with_type = true, bool explicit_types = false) {
    auto members = librepr::detail::to_tuple(obj);
    static_assert(type::size == std::tuple_size_v<decltype(members)>);

    return [with_type, explicit_types]<std::size_t... index>(std::index_sequence<index...>, auto const& tuple) {
      std::ostringstream stream;

      if (with_type) {
        stream << librepr::get_name<T>();
      }

      stream << '{';
      [[maybe_unused]] const char* delimiter = "";

      (((stream << delimiter
                << type::template get<index>::dump(std::get<index>(tuple), explicit_types, explicit_types)),
        delimiter = ", "),
       ...);
      stream << '}';
      return stream.str();
    }(std::make_index_sequence<type::size>{}, librepr::detail::to_tuple(obj));
  }

  static std::string layout() {
    return []<std::size_t... index>(std::index_sequence<index...>) {
      std::ostringstream stream;
      stream << '{';
      [[maybe_unused]] const char* delimiter = "";

      (((stream << delimiter << type::template get<index>::layout()), delimiter = ", "), ...);
      stream << '}';
      return stream.str();
    }(std::make_index_sequence<type::size>{});
  }
};

template <std::ranges::range T>
  requires std::constructible_from<std::initializer_list<typename T::value_type>>
struct Reflect<T> {
  using type = typename T::value_type;

  static std::string dump(T const& obj, bool with_type = true, bool explicit_types = false) {
    std::ostringstream list{};

    if (with_type) {
      list << librepr::get_name<T>();
    }

    list << '{';
    for (auto const& element : obj) {
      if (&element != &*std::begin(obj)) {
        list << ", ";
      }

      list << Reflect<type>::dump(element, explicit_types, explicit_types);
    }
    list << '}';
    return list.str();
  }

  static std::string layout() { return std::format("[{}]", librepr::get_name<typename T::value_type>()); }
};

template <typename T, std::size_t N>
struct Reflect<T[N]> {
  using type = T;

  static std::string dump(T const (&obj)[N], bool /*with_type*/ = false, bool explicit_types = false) {
    if constexpr (std::is_same_v<T, char const>) {
      // emit as string literal
      return librepr::repr(obj);
    } else {
      std::ostringstream list{};

      list << '{';
      for (std::size_t idx = 0; idx < N; ++idx) {
        if (idx != 0) {
          list << ", ";
        }

        list << Reflect<type>::dump(obj[idx], explicit_types, explicit_types);
      }
      list << '}';
      return list.str();
    }
  }

  static std::string layout() { return std::format("{}[{}]", librepr::get_name<T>(), N); }
};

template <typename T>
struct Reflect<T*> {
  using type = T;

  static std::string dump(T const* obj, bool /*with_type*/ = false, bool /*explicit_types*/ = false) {
    // TODO reflect if possible
    return std::format("({}){}", librepr::get_name<T*>(), librepr::repr(static_cast<void const*>(obj)));
  }

  static std::string layout() { return librepr::get_name<T*>(); }
};

template <>
struct Reflect<char const*> {
  static std::string dump(char const* obj, bool /*with_type*/ = false, bool /*explicit_types*/ = false) {
    return librepr::repr(obj);
  }

  static std::string layout() { return "char const*"; }
};

}  // namespace librepr
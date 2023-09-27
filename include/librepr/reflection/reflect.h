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

  static std::string dump(T const& obj) { 
    return librepr::repr(obj);
  }

  static std::string layout() { return librepr::get_name<T>(); }
};

template <detail::has_repr_member T>
struct Reflect<T> {
  using type = T;

  static std::string dump(T const& obj) { return obj.repr(); }

  static std::string layout() { 
    //TODO check for layout member function
    return librepr::get_name<T>();
  }
};

template <detail::pair_like T>
struct Reflect<T> {
  using type        = T;
  using first_type  = std::remove_cv_t<std::remove_reference_t<typename T::first_type>>;
  using second_type = std::remove_cv_t<std::remove_reference_t<typename T::second_type>>;

  static std::string dump(T const& obj) {
    return std::format("{{{}, {}}}", Reflect<first_type>::dump(obj.first), Reflect<second_type>::dump(obj.second));
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
  using type         = librepr::detail::TypeList<>::from_tuple<member_tuple>      \
                        ::template map_t<std::remove_reference>                   \
                        ::template map_t<std::remove_cv>                          \
                        ::template map<librepr::Reflect>;

  static std::string dump(T const& obj) {
    auto members = librepr::detail::to_tuple(obj);
    static_assert(type::size == std::tuple_size_v<decltype(members)>);

    return []<std::size_t... index>(std::index_sequence<index...>, auto const& tuple) {
      std::ostringstream stream;
      stream << '{';
      [[maybe_unused]] const char* delimiter = "";

      (((stream << delimiter << type::template get<index>::dump(std::get<index>(tuple))), delimiter = ", "), ...);
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

  static std::string dump(T const& obj) {
    std::ostringstream list{};

    list << '{';
    for (auto const& element : obj) {
      if (&element != &*std::begin(obj)) {
        list << ", ";
      }

      list << Reflect<type>::dump(element);
    }
    list << '}';
    return list.str();
  }

  static std::string layout() { return std::format("[{}]", librepr::get_name<typename T::value_type>()); }
};
}  // namespace librepr::detail
#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <tuple>

#include <librepr/detail/type_list.h>
#include <librepr/detail/concepts.h>

#include <librepr/reflection/detail/to_tuple.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
  requires std::is_aggregate_v<T> && (!detail::has_repr_member<T>) && (!std::is_array_v<T>)
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
}  // namespace librepr
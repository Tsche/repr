#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <tuple>
#include "librepr/reflection/visitor.h"

#include <librepr/detail/type_list.h>
#include <librepr/detail/concepts.h>

#include <librepr/reflection/detail/to_tuple.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
  requires (!detail::has_repr_member<T>  && std::is_aggregate_v<T> && !std::is_array_v<T>)
struct Reflect<T> {
  using member_tuple = decltype(librepr::detail::to_tuple(std::declval<T>()));
  static_assert(!std::is_same_v<member_tuple, void>);
  using type         = librepr::detail::TypeList<>::from_tuple<member_tuple>::template map_t<
      std::remove_reference>::template map_t<std::remove_cv>::template map<librepr::Reflect>;

  static void visit(auto&& visitor, T const& obj) {
    auto members = librepr::detail::to_tuple(obj);
    static_assert(type::size == std::tuple_size_v<decltype(members)>);

    ScopeGuard guard{visitor, std::type_identity<T>{}};
    
    []<std::size_t... index>(std::index_sequence<index...>, auto const& tuple, auto&& visitor_) {
      (type::template get<index>::visit(std::forward<decltype(visitor_)>(visitor_), std::get<index>(tuple)), ...);
    }(std::make_index_sequence<type::size>{}, librepr::detail::to_tuple(obj), std::forward<decltype(visitor)>(visitor));
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
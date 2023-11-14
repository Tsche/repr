#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <tuple>

#include <librepr/visitors/visitor.h>
#include <librepr/util/type_list.h>
#include <librepr/util/concepts.h>

#include <librepr/reflection/detail/to_tuple.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
  requires (!detail::has_repr_member<T>  && std::is_aggregate_v<T> && !std::is_array_v<T>)
struct Reflect<T> {
  using member_tuple = decltype(librepr::detail::to_tuple(std::declval<T>()));
  static_assert(!std::is_same_v<member_tuple, void>);
  using type = rebox<member_tuple, TypeList>::template map<std::remove_cvref_t>::template map<librepr::Reflect>;

  template <Visitor::Values V>
  static void visit(V&& visitor, T const& obj) {
    auto members = librepr::detail::to_tuple(obj);
    static_assert(type::size == std::tuple_size_v<decltype(members)>);

    ScopeGuard guard{visitor, std::type_identity<T>{}};
    
    []<std::size_t... index>(std::index_sequence<index...>, Visitor::Values auto&& visitor_, auto const& tuple) {
      (type::template get<index>::visit(std::forward<decltype(visitor_)>(visitor_), std::get<index>(tuple)), ...);
    }(std::make_index_sequence<type::size>{}, std::forward<V>(visitor), members);
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
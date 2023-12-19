#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <tuple>

#include <librepr/visitors/visitor.h>
#include <librepr/util/list.h>
#include <librepr/util/concepts.h>

#include <librepr/reflection/detail/to_tuple.h>
#include <librepr/type/member_names.h>

// TODO
#include <librepr/customization.h>
#include <librepr/customization/reflection.h>

namespace librepr {

template <typename T>
concept reflect_names = requires { requires librepr::Settings<T>::aggregate_member_names; };

template <typename T>
struct Reflect;

template <typename T>
  requires(std::is_aggregate_v<T> && !(std::is_array_v<T> || has_custom_members<T> || detail::has_repr_member<T>))
struct Reflect<T> {
  using member_tuple = decltype(librepr::detail::to_tuple(std::declval<T>()));
  static_assert(!std::is_same_v<member_tuple, void>);
  using type = pack::rebox<member_tuple, TypeList>::template map<std::remove_cvref_t>::template map<librepr::Reflect>;

  template <typename V>
  static void visit(V&& visitor, T const& obj) {
    auto members = librepr::detail::to_tuple(obj);
    static_assert(type::size == std::tuple_size_v<decltype(members)>);

    ScopeGuard guard{visitor, std::type_identity<T>{}};

    type::enumerate([&visitor, &members]<typename Member, std::size_t Index>() {
      if constexpr (reflect_names<T>) {
        Visit::member_name(visitor, std::string_view{librepr::member_name<T, Index>});
      }

      Member::visit(std::forward<decltype(visitor)>(visitor), std::get<Index>(members));
    });
  }

  static std::string layout() {
    auto output = std::string("{");
    type::enumerate([&output]<typename Member, std::size_t Index>() {
      if (Index != 0) {
        output += ", ";
      }

      output += Member::layout();
    });

    return output + '}';
  }
};
}  // namespace librepr
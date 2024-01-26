#pragma once
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

#include <librepr/util/list.h>
#include <librepr/util/pack.h>
#include <librepr/util/reftuple.h>
#include <librepr/util/concepts.h>
#include <librepr/name/member.h>

// TODO
#include <librepr/customization.h>
#include <librepr/customization/reflection.h>

#include "category.h"
#include "detail/arity.h"
#include "detail/to_reftuple.h"

namespace librepr {

template <typename T>
concept reflect_names = requires { requires librepr::Settings<T>::aggregate_member_names; };

template <typename T>
struct Reflect;

namespace category {

template <typename Member, typename Parent, std::size_t Index>
struct DataMember : Member {
  using descend     = Member;
  using type        = Member::type;
  using parent      = Parent;
  using parent_type = Parent::type;

  [[nodiscard]] std::string_view name() const { 
    return member_name<parent_type, Index>; 
  }

  using Member::visit;
};

}  // namespace category

template <typename T>
  requires(std::is_aggregate_v<T> && !(std::is_array_v<T> || has_custom_members<T> || detail::has_repr_member<T>))
struct Reflect<T> : category::Type<T> {
  constexpr static auto member_count = librepr::detail::arity<T>;

  using member_tuple = decltype(librepr::detail::to_reftuple(std::declval<T>()));
  static_assert(!std::is_same_v<member_tuple, void>, "Aggregate reflection failed");

  using member_types                = pack::rebox<member_tuple, TypeList>::template map<std::remove_reference_t>;
  using type                        = T;
  using members                     = member_types::template map<librepr::Reflect>;
  constexpr static bool can_descend = true;

  template <typename V>
  static void visit(V&& visitor) {
    members::enumerate(
        [&visitor]<typename M, std::size_t Index> { visitor(category::DataMember<M, Reflect, Index>{}); });
  }

  template <typename V>
  static void visit(V&& visitor, T& obj) {
    auto decomposed = librepr::detail::to_reftuple(obj);
    static_assert(members::size == decltype(decomposed)::size,
                  "Decomposed obj does not match reflected member amount.");

    members::enumerate([&visitor, &decomposed]<typename M, std::size_t Index> {
      visitor(category::Value<category::DataMember<M, Reflect, Index>>{librepr::get<Index>(decomposed)});
    });
  }
};
}  // namespace librepr
#pragma once
#include "concepts.h"
#include "member_ptr.h"
#include <librepr/macro/warning.h>

namespace librepr::util {

template <is_member_pointer Ptr, is_enum Tag>
struct UnionMember {
  using member_ptr_info = MemberPtr<Ptr>;
  static_assert(!member_ptr_info::is_function, "Member functions are not supported.");
  static_assert(std::is_union_v<typename member_ptr_info::class_type>, "Member's class type isn't a union.");
  using class_type  = typename MemberPtr<Ptr>::class_type;
  using member_type = typename MemberPtr<Ptr>::return_type;

  Ptr member;
  Tag tag;
};

template <is_member_pointer Ptr, is_enum Tag>
UnionMember(Ptr, Tag) -> UnionMember<Ptr, Tag>;

namespace detail {
LIBREPR_WARNING_PUSH
LIBREPR_WARNING_DISABLE(GCC, "-Wnon-template-friend")

template <class C, auto E>
struct ByTag {
  constexpr friend auto get_union_accessor_adl(ByTag);
};

template <class C, typename T>
struct ByMemberType {
  constexpr friend auto get_union_accessor_adl(ByMemberType);
};

template <UnionMember Member>
struct UnionEntry {
  using class_type = typename decltype(Member)::class_type;
  constexpr friend auto get_union_accessor_adl(ByTag<class_type, Member.tag>) { return Member; }
  constexpr friend auto get_union_accessor_adl(ByMemberType<class_type, typename decltype(Member)::member_type>) {
    return Member;
  }
};

LIBREPR_WARNING_POP

template <UnionMember... Ts>
consteval bool enroll_union_members() {
  ((void)detail::UnionEntry<Ts>{}, ...);
  return true;
}

template <UnionMember... Ts>
struct EnableUnionImpl;

template <UnionMember T, UnionMember... Ts>
struct EnableUnionImpl<T, Ts...> {
  static_assert(all_same<decltype(T.tag)>, "Inconsistent tag type used.");
  static_assert(all_same<typename decltype(T)::class_type>, "Not all member pointers belong to the same union.");

  constexpr static bool value = enroll_union_members<T, Ts...>();
};
}  // namespace detail

template <UnionMember... Accessors>
concept EnableUnion = detail::EnableUnionImpl<Accessors...>::value;

template <typename Union, auto E>
constexpr inline auto get_union_accessor = get_union_accessor_adl(detail::ByTag<Union, E>{}).member;

template <typename Union, typename Member>
constexpr inline auto get_union_tag = get_union_accessor_adl(detail::ByMemberType<Union, Member>{}).tag;

template <typename Union, typename Member>
using union_member_type = typename decltype(get_union_accessor_adl(detail::ByMemberType<Union, Member>{}))::member_type;

template <typename Union, typename Member>
concept is_tagged_union = requires { get_union_accessor_adl(detail::ByMemberType<Union, Member>{}); };

}  // namespace librepr::util

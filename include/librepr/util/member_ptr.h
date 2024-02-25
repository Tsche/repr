#pragma once

namespace librepr::util {
template <typename T>
struct MemberPtr;

template <typename R, typename C>
struct MemberPtr<R C::*>{
  constexpr static bool is_function = false;
  using type = R C::*;
  using return_type = R;
  using class_type = C;
};

template <typename R, typename C, typename... Args>
struct MemberPtr<R (C::*)(Args...)>{
  constexpr static bool is_function = true;
  using type = R C::*;
  using return_type = R;
  using class_type = C;
  // using argument_types = librepr::TypeList<Args..>;
};
// TODO various function signatures

template <typename T>
concept is_member_pointer = requires { typename MemberPtr<T>;};
}
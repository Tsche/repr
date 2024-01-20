#pragma once

#include <string_view>
#include <utility>
#include <array>
#include <librepr/macro/warning.h>
#include <librepr/customization/reflection.h>
#include <librepr/ctvi/ctvi.h>
#include <librepr/customization/members.h>

#include <librepr/util/fake_obj.h>
#include <librepr/util/string.h>

#include <librepr/reflection/detail/arity.h>
#include <librepr/reflection/detail/to_tuple.h>
#include <librepr/macro/platform.h>

namespace librepr {
namespace detail {

#if USING(LIBREPR_COMPILER_CLANG)
// clang-format off
#define LIBREPR_SUBOBJECT_PTR(...) Wrap{__VA_ARGS__}
// clang-format on
#else
#define LIBREPR_SUBOBJECT_PTR(...) __VA_ARGS__
#endif

template <auto>
struct MemberName;

template <typename C, typename T, T C::*ptr>
struct MemberName<ptr> {
  using type = C;

  constexpr static auto value =
#if USING(LIBREPR_COMPILER_MSVC)

      // MSVC unfortunately returns pointer to member template arguments into offsets
      // like `pointer-to-member(0x0)`. Using the aggregate member name reflection trick yields
      // `&fake_obj<struct Test>->value->member` instead, which is usable here.

      librepr::ctvi::detail::name_from_subobject<C, std::addressof(fake_obj<C>.value.*ptr)>();
#else
      librepr::ctvi::detail::name_from_member_ptr<ptr>();
#endif
};

template <Member info>
struct MemberName<info> {
  using type = typename decltype(info)::class_type;

  constexpr static auto member_name() {
    if constexpr (!info.name.empty()) {
      return info.name;
    } else if constexpr (!std::is_member_function_pointer_v<decltype(info.value.value)> &&
                         !std::is_function_v<std::remove_pointer_t<decltype(info.value.value)>>) {
      // don't extract names of accessor functions - those will probably be incorrect
      return MemberName<info.value.value>{}.value;
    } else {
      return const_string<0>{};
    }
  }
  constexpr static auto value = member_name();
};

LIBREPR_WARNING_PUSH
LIBREPR_WARNING_DISABLE(CLANG, -Wundefined - var - template)

// This technique has been described by @schaumb
// For explanations see
// https://github.com/boostorg/pfr/issues/150#issuecomment-1831500004
// https://github.com/boostorg/pfr/issues/90#issuecomment-1589530490

template <std::size_t Idx>
constexpr auto member_ptr(auto const& obj) {
  return std::addressof(std::get<Idx>(to_tuple(obj)));
}

template <typename T, std::size_t Idx>
  requires(std::is_aggregate_v<T> && !std::is_array_v<T>)
inline constexpr auto raw_member_name =
    librepr::ctvi::detail::name_from_subobject<T, LIBREPR_SUBOBJECT_PTR(member_ptr<Idx>(fake_obj<T>.value))>();

LIBREPR_WARNING_POP

template <typename T>
  requires(std::is_aggregate_v<T> && !std::is_array_v<T>)
inline constexpr auto raw_member_names = []<std::size_t... Idx>(std::index_sequence<Idx...>) {
  return std::array{std::string_view{raw_member_name<T, Idx>}...};
}(std::make_index_sequence<arity<T>>{});

template <auto Accessor>
constexpr inline auto custom_member_name = MemberName<Accessor>::value;

template <typename T>
constexpr auto get_member_names() {
  // TODO

  using error = std::array<std::string_view, 0>;
  if constexpr (has_custom_members<T>) {
    if constexpr (Settings<T>::members != 0) {
      return Settings<T>::members::invoke(
          []<Member... members>() { return std::array{std::string_view{custom_member_name<members>}...}; });
    } else {
      return error{};
    }
  } else if constexpr (std::is_aggregate_v<T> && !std::is_array_v<T>) {
    constexpr auto member_count = arity<T>;
    if constexpr (member_count != 0 && requires { raw_member_name<T, 0>; }) {
      return []<std::size_t... Idx>(std::index_sequence<Idx...>) {
        return std::array{std::string_view{raw_member_name<T, Idx>}...};
      }(std::make_index_sequence<member_count>{});

    } else {
      return error{};
    }
  } else {
    return error{};
  }
}

}  // namespace detail

template <typename T>
constexpr inline auto member_names = detail::get_member_names<T>();

template <typename T, std::size_t Idx>
  requires(Idx <= member_names<T>.size())
constexpr inline auto member_name = member_names<T>[Idx];
}  // namespace librepr
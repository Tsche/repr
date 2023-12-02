#pragma once
#include <type_traits>
#include <bit>
#include <concepts>

#include <librepr/ctvi/ctvi.h>
#include <librepr/util/strings.h>

namespace librepr::ctei {
enum class EnumKind { Linear, Flags, Empty };

template <typename T, EnumKind Kind>
[[nodiscard]] constexpr auto to_underlying(auto idx) noexcept -> std::underlying_type_t<T> {
  using underlying = std::underlying_type_t<T>;
  if constexpr (Kind == EnumKind::Flags) {
    if (idx == 0) {
      return 0;
    } else {
      return underlying{1} << static_cast<underlying>(idx - 1);
    }
  } else {
    return static_cast<underlying>(idx);
  }
}

template <typename T, auto Value>
[[nodiscard]] constexpr T to_enum() noexcept {
  if constexpr (std::same_as<T, decltype(Value)>) {
    return Value;
  } else {
    using underlying = std::underlying_type_t<T>;
    // https://discourse.llvm.org/t/clang-16-notice-of-potentially-breaking-changes/65562
    // GCC seems to warn for this as well
    // TODO use this new diagnostic for search once https://github.com/llvm/llvm-project/issues/57176 is fixed

    // library UB does not need to be diagnosed
    // (https://eel.is/c++draft/expr.const#5.31) hence we can use std::bit_cast
    // here to bypass the miscompile and possible warnings
    return std::bit_cast<T>(static_cast<underlying>(Value));
  }
}

namespace detail {
template <typename T, auto Value>
[[nodiscard]] constexpr auto get_enum_name() noexcept {
  constexpr auto name = ctvi::value<to_enum<T, Value>()>;
  if constexpr (!name.empty() && name[0] != '(') {
    return name;
  } else {
    return std::array{'\0'};
  }
}
template <typename T, auto Value>
inline constexpr auto enum_name_raw = get_enum_name<T, Value>();
}  // namespace detail

template <typename T, auto Value>
inline constexpr auto enum_name = std::string_view{detail::enum_name_raw<T, Value>.data()};

template <auto V>
[[nodiscard]] consteval auto dump_quick() noexcept {
#if defined(_MSC_VER)
  constexpr auto signature = std::string_view{__FUNCSIG__};
  // TODO find prefix for MSVC
  constexpr std::size_t start = sizeof("TODO") - 1;
#else
  constexpr auto signature = std::string_view{__PRETTY_FUNCTION__};
#if defined(__clang__)
  //
  constexpr std::size_t start = sizeof("auto librepr::ctei::dump_quick() [V = ") - 1;
#elif defined(__GNUC__)
  //
  constexpr std::size_t start = sizeof("consteval auto librepr::ctei::dump_quick() [with auto V = ") - 1;
#elif defined(_MSC_VER)
#endif
#endif
  constexpr std::size_t end = signature.length() - 1;

  return signature.substr(start, end - start);
}

template <auto... V>
[[nodiscard]] consteval auto dump_list() noexcept {
#if defined(_MSC_VER)
  constexpr auto signature = std::string_view{__FUNCSIG__};
  // TODO find prefix for MSVC
  constexpr std::size_t start = sizeof("TODO") - 1;
#else
  constexpr auto signature = std::string_view{__PRETTY_FUNCTION__};
#if defined(__clang__)
  constexpr std::size_t start = sizeof("auto librepr::ctei::dump_list() [V = <") - 1;
#elif defined(__GNUC__)
  constexpr std::size_t start = sizeof("consteval auto librepr::ctei::dump_list() [with auto ...V = {") - 1;
#endif
#endif
  constexpr std::size_t end = signature.length() - 2;

  return signature.substr(start, end - start);
}

template <typename T, auto Value>
[[nodiscard]] constexpr bool is_enum_value() {
  // accessing the underlying array here directly seems to perform slightly better
  // constexpr auto name = detail::enum_name_raw<T, Value>;
  constexpr auto name = dump_quick<std::bit_cast<T>(static_cast<std::underlying_type_t<T>>(Value))>();
  return !name.empty() && name[0] != '(';
}

}  // namespace librepr::ctei
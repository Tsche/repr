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
    // here to bypass the miscompile
    // return __builtin_bit_cast(T, Value); //better debug output
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
}

template <typename T, auto Value>
inline constexpr auto enum_name = std::string_view{detail::enum_name_raw<T, Value>};

template <typename T, auto Value>
[[nodiscard]] constexpr bool is_enum_value() {
  // accessing the underlying array here directly seems to perform slightly better
  constexpr auto name = detail::enum_name_raw<T, Value>;
  return !name.empty() && name[0] != '\0';
}

}  // namespace librepr::ctei
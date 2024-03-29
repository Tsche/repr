#pragma once
#include <cstddef>
#include <type_traits>

namespace librepr {

template <typename T>
struct is_literal {
  // don't print type names for fundamental types and string literals
  constexpr static bool value = std::is_fundamental_v<T> || std::is_same_v<T, char const*>;
};

template <typename T, std::size_t N>
struct is_literal<T[N]> {
  // don't print type names for arrays
  constexpr static bool value = true;
};

template <typename T>
requires std::is_enum_v<T>
struct is_literal<T> {
  // don't print type names for enums
  constexpr static bool value = true;
};

template <typename T>
inline constexpr bool is_literal_v = is_literal<T>::value;
}
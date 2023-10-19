#pragma once
#include <type_traits>

namespace librepr {

template <typename T>
struct is_literal {
  static constexpr bool value = std::is_fundamental_v<T> || std::is_same_v<T, char const*>;
};

template <typename T, std::size_t N>
struct is_literal<T[N]> {
  static constexpr bool value = true;
};

template <typename T>
inline constexpr bool is_literal_v = is_literal<T>::value;
}
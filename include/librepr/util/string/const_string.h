#pragma once
#include <algorithm>
#include <cstddef>
#include <string_view>

namespace librepr {

template <std::size_t N>
struct [[nodiscard]] const_string {
  char value[N + 1]{};
  constexpr static auto size = N;

  constexpr const_string() = default;

  constexpr explicit(false) const_string(char const (&literal)[N + 1]) {  // NOLINT
    std::copy(literal, literal + N, std::begin(value));
  }

  constexpr explicit const_string(std::string_view data) { std::copy(begin(data), end(data), std::begin(value)); }
  [[nodiscard]] constexpr explicit operator std::string_view() const noexcept { return std::string_view{value}; }
  [[nodiscard]] constexpr explicit operator char const*() const noexcept { return value; }

  [[nodiscard]] constexpr auto to_sv() const noexcept { return std::string_view{value}; }
  [[nodiscard]] constexpr bool empty() const noexcept { return size == 0; }

  [[nodiscard]] constexpr char const& operator[](std::size_t Idx) const { return value[Idx]; }
  friend constexpr auto operator==(const_string lhs, std::string_view rhs) noexcept { return rhs == lhs.data; }
  constexpr auto length() const { return std::char_traits<char>::length(value); }
};

template <std::size_t N>
const_string(char const (&)[N]) -> const_string<N - 1>;

template <const_string Str>
constexpr auto shrink_to_fit() {
  constexpr auto size = Str.length();
  return const_string<size>{Str.value};
}
}  // namespace librepr
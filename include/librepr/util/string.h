#pragma once

#include <string_view>
#include <string>
#include <array>
namespace librepr {

template <std::size_t N>
struct [[nodiscard]] const_string {
  char value[N + 1]{};
  constexpr static auto size = N;

  constexpr const_string() = default;

  constexpr explicit(false) const_string(char const (&literal)[N + 1]) {  // NOLINT
    std::copy(literal, literal + N, std::begin(value));
  }
  
  constexpr explicit const_string(std::string_view data) { 
    std::copy(data.data(), data.data() + N, std::begin(value)); 
  }

  [[nodiscard]] constexpr explicit operator std::string_view() const noexcept { return std::string_view{value}; }
  [[nodiscard]] constexpr explicit operator char const*() const noexcept { return value; }

  [[nodiscard]] constexpr auto to_sv() const noexcept { return std::string_view{value}; }
  [[nodiscard]] constexpr bool empty() const noexcept { return size == 0; }

  [[nodiscard]] constexpr char const& operator[](std::size_t Idx) const { return value[Idx]; }
  friend constexpr auto operator==(const_string lhs, std::string_view rhs) noexcept { return rhs == lhs.data; }
};

template <std::size_t N>
const_string(char const (&)[N]) -> const_string<N - 1>;
}  // namespace librepr::detail
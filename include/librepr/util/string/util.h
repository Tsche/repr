#pragma once
#include <cstddef>
#include <concepts>
#include <string>

namespace librepr::util {
template <typename T>
concept string_like = requires(T& obj) {
  obj.size();
  { obj.data() } -> std::convertible_to<const char*>;
};

template <std::size_t N>
constexpr std::size_t strsize(const char(&)[N]) {
  return (N - 1U);
}

constexpr std::size_t strsize(const char* str) {
  return std::char_traits<char>::length(str);
}

template <string_like T>
constexpr std::size_t strsize(T&& obj) {
  return obj.size();
}
}  // namespace librepr::util
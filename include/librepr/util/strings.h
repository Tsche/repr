#pragma once
#include <cstddef>
#include <concepts>
#include <string>

namespace librepr::detail {
template <typename T>
concept string_like = requires(T& t) {
  t.size();
  { t.data() } -> std::convertible_to<const char*>;
};

template <std::size_t N>
constexpr std::size_t strsize(const char(&)[N]) {
  return (N - 1U);
}

constexpr std::size_t strsize(const char* str) {
  return std::char_traits<char>::length(str);
}

template <string_like T>
constexpr std::size_t strsize(T&& t) {
  return t.size();
}

inline void remove_all(std::string& text, const std::string& from) {
  for (auto at = text.find(from, 0); at != text.npos; at = text.find(from, at)) {
    text.erase(at, from.length());
  }
}
}  // namespace librepr::detail
#pragma once

#include <string>
#include <array>
namespace librepr::detail {
inline void remove_all(std::string& text, const std::string& from) {
  for (auto at = text.find(from, 0); at != text.npos; at = text.find(from, at)) {
    text.erase(at, from.length());
  }
}

template <std::size_t... Idx>
consteval auto sv_to_array(std::string_view str, std::index_sequence<Idx...>) noexcept
    -> std::array<char, (sizeof...(Idx) + 1)> {
  return {{str[Idx]..., '\0'}};
}

}  // namespace librepr::detail
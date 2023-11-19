#pragma once
#include <string_view>
#include <array>
#include <utility>

namespace librepr::ctvi {
namespace detail {
template <std::size_t... Idx>
consteval auto sv_to_array(std::string_view str, std::index_sequence<Idx...>) noexcept
    -> std::array<char, (sizeof...(Idx) + 1)> {
  return {{str[Idx]..., '\0'}};
}

template <auto V>
[[nodiscard]] constexpr auto get_ctvi() noexcept {
  // TODO Windows implementation using __FUNCSIG__
  constexpr auto prefix = std::string_view{"V = "};
  constexpr auto suffix = std::string_view{"]\0"};

  constexpr auto signature    = std::string_view{__PRETTY_FUNCTION__};
  constexpr std::size_t start = signature.find(prefix) + prefix.size();
  constexpr std::size_t end   = signature.find(suffix, start);

  constexpr auto value = signature.substr(start, end - start);
  return sv_to_array(value, std::make_index_sequence<value.size()>{});
}
}  // namespace detail

template <auto V>
constexpr inline auto value = detail::get_ctvi<V>();

}  // namespace librepr::ctvi
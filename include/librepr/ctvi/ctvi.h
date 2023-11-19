#pragma once
#include <string_view>
#include <array>
#include <utility>

#include <librepr/util/strings.h>

namespace librepr::ctvi {
template <auto V>
[[nodiscard]] constexpr auto get_ctvi() noexcept {
  // TODO Windows implementation using __FUNCSIG__
  constexpr auto prefix = std::string_view{"V = "};
  constexpr auto suffix = std::string_view{"]\0"};

  constexpr auto signature    = std::string_view{__PRETTY_FUNCTION__};
  constexpr std::size_t start = signature.find(prefix) + prefix.size();
  constexpr std::size_t end   = signature.find(suffix, start);

  constexpr auto value = signature.substr(start, end - start);
  return detail::sv_to_array(value, std::make_index_sequence<value.size()>{});
}

template <auto V>
constexpr inline auto value = get_ctvi<V>();

}
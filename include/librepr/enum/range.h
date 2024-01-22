#pragma once
#include <array>
#include <cstddef>
#include <utility>
#include <string_view>
#include <bit>
#include "util.h"


namespace librepr::ctei {
template <auto First, auto Last = First>
struct Range {
  static_assert(Last >= First);

  constexpr static auto size = Last - First + 1;
  constexpr static auto min  = First;
  constexpr static auto max  = Last;

  template <auto Idx>
  requires (Idx <= size)
  constexpr static auto get = Idx + First;

  template <std::size_t amount = 1>
  using expand = Range<First, max + static_cast<decltype(max)>(amount)>;

private:
  template <typename T, EnumKind Kind, std::size_t... Idx>
  static consteval auto get_enum_names(std::index_sequence<Idx...>){
    return std::array{std::string_view{enum_name<T, to_underlying<T, Kind>(Range::template get<Idx>)>}...};
  }

public:
  template <typename T, EnumKind Kind>
  constexpr static auto enum_names = get_enum_names<T, Kind>(std::make_index_sequence<size>{});

  [[nodiscard]] constexpr static bool is_binary_powers() noexcept{
    if constexpr (min >= 0 && max <= 2) {
      // special case ranges containing only 0-2
      return true;
    } else if constexpr (std::has_single_bit(static_cast<std::size_t>(min))) {
      // offset is power of 2 - range must contain element at (offset + 0)
      return size == 1;
    }
    return false;
  }

  [[nodiscard]] constexpr static bool contains(auto value) noexcept { 
    return value >= min && value <= max; 
  }
};
}  // namespace librepr::ctei
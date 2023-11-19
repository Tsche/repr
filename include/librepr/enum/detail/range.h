#pragma once
#include <bit>

namespace librepr::ctei {
  template <auto First, auto Last = First>
struct Range {
  static_assert(Last >= First);

  static constexpr auto size  = Last - First + 1;
  static constexpr auto min = First;
  static constexpr auto max  = Last;

  template <auto Idx>
  static constexpr auto get = Idx + First;

  using expand = Range<First, max + 1>;

  static constexpr bool is_binary_powers() {
    if constexpr (min >= 0 && min < 2) {
      // special case ranges containing only 0-2
      return max <= 2;
    } else if constexpr (std::has_single_bit(static_cast<std::size_t>(min))) {
      // offset is power of 2 - range must contain element at (offset + 0)
      return size == 1;
    }
    return false;
  }

  static bool contains(auto value) { return value >= min && value <= max; }
};
}
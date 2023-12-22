#pragma once
#include <array>
#include <limits>
#include <string_view>
#include <type_traits>
#include <algorithm>
#include <utility>

#include <librepr/customization.h>
#include <librepr/macro/default.h>

#include "range.h"
#include "util.h"
#include "range_list.h"
#include "accessor.h"

namespace librepr::ctei {

constexpr std::size_t find_first(bool const* array, std::size_t min, std::size_t max, bool value) {
  for (auto idx = min; idx < max; ++idx) {
    if (array[idx] == value) {
      return idx;
    }
  }
  return max;
}

template <auto array, auto Offset, typename Acc, auto ArrayOffset = 0>
static consteval auto rangify() {
  using underlying = decltype(Offset);
  if constexpr (array.empty()) {
    return Acc{};
  } else {
    constexpr auto first = find_first(array.data(), ArrayOffset, array.size(), true);
    if constexpr (first == array.size()) {
      return Acc{};
    } else {
      constexpr auto last = find_first(array.data(), first, array.size(), false);
      return rangify<array, Offset,
                     typename Acc::template add_range<
                         Range<Offset + static_cast<underlying>(first), Offset + static_cast<underlying>(last - 1)>>,
                     last>();
    }
  }
}

template <typename T>
constexpr T clamp(auto value) {
  constexpr auto min = std::numeric_limits<T>::min();
  constexpr auto max = std::numeric_limits<T>::max();
  if (std::cmp_less(value, min)) {
    return min;
  }
  return std::cmp_less(max, value) ? max : static_cast<T>(value);
}

template <typename T>
  requires std::is_enum_v<T>
struct Search {
  using underlying = std::underlying_type_t<T>;

  template <underlying Offset, underlying Max>
  static constexpr auto search_chunk() {
    return []<std::size_t... Idx>(std::index_sequence<Idx...>) {
      return std::array{
          dump_quick<std::bit_cast<T>(static_cast<underlying>(Idx) + Offset)>()[0] != '(' ...
          // is_enum_value<T, Offset + Idx>()...
      };
    }(std::make_index_sequence<Max - Offset>{});
  }

  template <underlying Offset, underlying Max>
  static constexpr auto search_chunk_multi() {
    auto array = std::array<bool, Max - Offset>{};

    auto list = []<std::size_t... Idx>(std::index_sequence<Idx...>) {
      return dump_list<std::bit_cast<T>(static_cast<underlying>(Idx) + Offset)...>();
    }(std::make_index_sequence<Max - Offset>{});

    array[0]            = list[0] != '(';
    std::size_t out_idx = 1;

    for (std::size_t idx = 0; idx < list.length(); ++idx) {
      if (list[idx] == ',') {
        // TODO check if all compilers insert a space after the comma
        array[out_idx++] = list[idx + 2] != '(';
      }
    }
    return array;
  }

  template <underlying Offset, underlying Max, typename Acc = RangeList<>, underlying N = 0>
  static consteval auto search_range() {
    if constexpr (N == Max) {
      return Acc{};
    } else {
      constexpr auto index = Offset + N;
      return search_range<Offset, Max, typename Acc::template try_one<T, index>, N + 1>();
    }
  }

  template <underlying Offset, underlying Max, typename Acc = RangeList<>>
  using search_recursive = decltype(search_range<Offset, Max, Acc>());

  template <underlying Offset, underlying Max, typename Acc = RangeList<>>
  using search_chunked = decltype(rangify<search_chunk<Offset, Offset + Max>(), Offset, Acc>());

  template <underlying Offset, underlying Max, typename Acc = RangeList<>>
  using search_fast = decltype(rangify<search_chunk_multi<Offset, Offset + Max>(), Offset, Acc>());

  template <underlying Offset, underlying Max, typename Acc = RangeList<>>
  using do_search =
#if USING(REPR_ENUM_RECURSIVE_SEARCH)
      search_recursive<Offset, Max, Acc>;
#elif USING(REPR_ENUM_FAST_SEARCH)
      search_fast<Offset, Max, Acc>;
#else
      search_chunked<Offset, Max, Acc>;
#endif

  template <auto Offset, auto Max, auto ChunkSize = REPR_ENUM_CHUNKSIZE, typename Acc = RangeList<>, auto N = 0>
  static consteval auto search_ranges() {
    constexpr auto offset = Offset + N * ChunkSize;

    if constexpr (offset >= Max) {
      return Acc{};
    } else if constexpr (offset + ChunkSize > Max) {
      return do_search<offset, Max - offset, Acc>{};
    } else {
      return search_ranges<Offset, Max, ChunkSize, do_search<offset, ChunkSize, Acc>, N + 1>();
    }
  }

  static consteval auto reflected_min() {
    if constexpr (has_search_range<T>) {
      return clamp<underlying>(Settings<T>::search_range::min);
    }

    if constexpr (std::is_signed_v<underlying>) {
      return clamp<underlying>(REPR_ENUM_MIN_SIGNED);
    } else {
      return clamp<underlying>(REPR_ENUM_MIN_UNSIGNED);
    }
  }

  static consteval auto reflected_max() {
    if constexpr (has_search_range<T>) {
      return clamp<underlying>(Settings<T>::search_range::max);
    }

    if constexpr (std::is_signed_v<underlying>) {
      return clamp<underlying>(REPR_ENUM_MAX_SIGNED);
    } else {
      return clamp<underlying>(REPR_ENUM_MAX_UNSIGNED);
    }
  }

  template <auto N = std::numeric_limits<underlying>::digits - 1>
    requires std::is_unsigned_v<underlying>
  static consteval int largest_flag() {
    if constexpr (is_enum_value<T, 1ULL << N>()) {
      return N + 1;
    }

    if constexpr (N == 0) {
      if constexpr (is_enum_value<T, 0>()) {
        return 0;
      }
      return -1;
    } else {
      return largest_flag<N - 1>();
    }
  }

  static consteval auto search() {
    using enum EnumKind;
    constexpr auto min  = reflected_min();
    constexpr auto max  = reflected_max();
    using linear_search = decltype(search_ranges<min, max>());

    if constexpr (linear_search::size == 0) {
      return Accessor<T, Empty>{};
    } else {
      if constexpr (std::is_unsigned_v<underlying> && linear_search::is_binary_powers()) {
        // all found values were flag-like so far, try all bits
        constexpr auto flag_max = largest_flag();
        if constexpr (flag_max == -1) {
          // something went terribly wrong - none of the powers of 2 are valid values
          // assume the range was empty to begin with
          return Accessor<T, Empty>{};
        } else {
          return Accessor<T, Flags, RangeList<Range<0, flag_max>>>{};
        }
      } else {
        return Accessor<T, Linear, linear_search>{};
      }
    }
  }
};

template <typename T>
  requires std::is_enum_v<T>
using dump_enum = decltype(Search<T>::search());

}  // namespace librepr::ctei

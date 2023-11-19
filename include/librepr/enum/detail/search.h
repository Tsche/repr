#pragma once
#include <array>
#include <limits>
#include <string_view>
#include <type_traits>
#include <algorithm>

#include <librepr/customization.h>
#include <librepr/detail/default.h>

#include "range.h"
#include "util.h"
#include "layout.h"

namespace librepr::ctei {

template <typename T, EnumKind Kind, typename Structure = Layout<>>
  requires std::is_enum_v<T>
struct EnumInfo {
  using structure            = Structure;
  using underlying           = std::underlying_type_t<T>;
  constexpr static auto kind = Kind;

private:
  template <auto Max, auto Idx = 0>
  static consteval void copy_ranges(auto& array, underlying offset) {
    using range = Structure::list::template get<Idx>;

    constexpr auto buffer = []<std::size_t... SubIdx>(std::index_sequence<SubIdx...>) {
      return std::array<std::string_view, range::size>{
          enum_name<T, to_underlying<T, Kind>(range::template get<SubIdx>)>.data()...};
    }(std::make_index_sequence<range::size>{});

    std::copy(std::begin(buffer), std::end(buffer), std::next(std::begin(array), offset));

    if constexpr (Idx + 1 < Max) {
      // copy next range
      copy_ranges<Max, Idx + 1>(array, range::size + offset);
    }
  }

  static consteval auto get_enum_names() {
    auto buffer = std::array<std::string_view, Structure::full_size>{};
    if constexpr (Structure::size > 0) {
      copy_ranges<Structure::size>(buffer, 0);
    }

    return buffer;
  }

public:
  static constexpr auto names = get_enum_names();

  template <std::size_t Idx = 0>
  constexpr static std::string_view search_name(underlying value)
    requires(Kind == EnumKind::Linear)
  {
    using range = Structure::list::template get<Idx>;

    if (range::contains(value)) {
      auto offset = value - range::min;
      // TODO assert offset is >= 0
      return names[std::size_t(offset)];
    }

    if constexpr (Idx + 1 < Structure::size) {
      // we haven't yet found, advance to next range
      // TODO could do binary search instead
      return search_name<Idx + 1>(value);
    } else {
      return {};
    }
  }

  static std::string_view search_name(underlying value)
    requires(Kind == EnumKind::Flags)
  {
    using range = Structure::list::template get<0>;

    if (value == 0) {
      // special case 0
      return names[0];
    }

    if (!std::has_single_bit(value)) {
      // more than one flag
      return {};
    }

    auto width = std::bit_width(value);
    if (width > range::max) {
      // value too large
      return {};
    }
    auto offset = std::countr_zero(value) + 1;
    // TODO assert offset is >= 0
    return names[std::size_t(offset)];
  }

  static std::string_view search_name(underlying /*value*/)
    requires(Kind == EnumKind::Empty)
  {
    return {};
  }

  constexpr static std::string_view get_name(auto value) {
    auto underlying = static_cast<std::underlying_type_t<T>>(value);
    return search_name(underlying);
  }

  constexpr static auto get_names() { return names; }

  constexpr static auto get_names()
    requires(Kind == EnumKind::Flags)
  {
    constexpr auto valid_count =
        std::count_if(std::begin(names), std::end(names), [](std::string_view element) { return !element.empty(); });

    auto buffer = std::array<std::string_view, valid_count>{};
    auto idx    = 0U;
    for (auto&& element : names) {
      if (element.empty()) {
        continue;
      }

      buffer[idx++] = element;
    }
    return buffer;
  }
};

template <typename T>
  requires std::is_enum_v<T>
struct EnumRange;

template <typename T>
concept has_enum_range = std::is_enum_v<T> && requires(T obj) {
  { EnumRange<T>::min };
  { EnumRange<T>::max };
};

template <typename T>
  requires std::is_enum_v<T>
struct EnumSearch {
  template <auto Offset, auto Max, typename Acc = Layout<>, auto N = 0>
  static consteval auto search_range() {
    if constexpr (N == Max) {
      return Acc{};
    } else {
      constexpr auto index = Offset + N;
      return search_range<Offset, Max, typename Acc::template try_one<T, index>, N + 1>();
    }
  }

  template <auto Offset, auto Max, auto ChunkSize = REPR_ENUM_CHUNKSIZE, typename Acc = Layout<>, auto N = 0>
  static consteval auto search_ranges() {
    constexpr auto offset = Offset + N * ChunkSize;

    if constexpr (offset >= Max) {
      return Acc{};
    } else if constexpr (offset + ChunkSize > Max) {
      return search_range<offset, Max - offset, Acc>();
    } else {
      return search_ranges<Offset, Max, ChunkSize, decltype(search_range<offset, ChunkSize, Acc>()), N + 1>();
    }
  }

  using underlying = std::underlying_type_t<T>;

  static consteval auto clamp(underlying value) {
    constexpr auto min = std::numeric_limits<underlying>::min();
    constexpr auto max = std::numeric_limits<underlying>::max();

    return value < min ? min : value > max ? max : value;
  }

  static consteval auto reflected_min() {
    if constexpr (std::is_signed_v<underlying>) {
      return clamp(REPR_ENUM_MIN_SIGNED);
    } else {
      return clamp(REPR_ENUM_MIN_UNSIGNED);
    }
  }

  static consteval auto reflected_max() {
    if constexpr (std::is_signed_v<underlying>) {
      return clamp(REPR_ENUM_MAX_SIGNED);
    } else {
      return clamp(REPR_ENUM_MAX_UNSIGNED);
    }
  }

  static consteval auto reflected_min()
    requires has_enum_range<T>
  {
    return EnumRange<T>::min;
  }

  static consteval auto reflected_max()
    requires has_enum_range<T>
  {
    return EnumRange<T>::max;
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
    constexpr auto min  = reflected_min();
    constexpr auto max  = reflected_max();
    using linear_search = decltype(search_ranges<min, max>());

    if constexpr (linear_search::size == 0) {
      return EnumInfo<T, EnumKind::Empty>{};
    } else {
      if constexpr (std::is_unsigned_v<underlying> && linear_search::is_flag_like) {
        // all found values were flag-like so far, try all bits
        constexpr auto flag_max = largest_flag();
        if constexpr (flag_max == -1) {
          // something went terribly wrong - none of the powers of 2 are valid values
          // assume the range was empty to begin with
          return EnumInfo<T, EnumKind::Empty>{};
        } else {
          return EnumInfo<T, EnumKind::Flags, Layout<Range<0, flag_max>>>{};
        }
      } else {
        return EnumInfo<T, EnumKind::Linear, linear_search>{};
      }
    }
  }
};

template <typename T>
  requires std::is_enum_v<T>
using dump_enum = decltype(EnumSearch<T>::search());

}  // namespace librepr::ctei

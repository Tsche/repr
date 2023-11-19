#pragma once
#include <string>
#include <type_traits>
#include <array>
#include <string_view>
#include <algorithm>

#include <librepr/util/type_list.h>

#include "util.h"

namespace librepr::ctei {

template <typename T, EnumKind Kind, typename Structure = TypeList<>>
  requires std::is_enum_v<T>
struct Accessor {
  using structure            = Structure;
  using underlying           = std::underlying_type_t<T>;
  constexpr static auto kind = Kind;

private:
  static consteval auto get_enum_names() {
    if constexpr (structure::size == 0) {
      return std::array<std::string_view, 0>{};
    } else {
      auto constexpr full_size = structure::invoke([]<typename... Range>() { return ((Range::size) + ...); });
      auto buffer = std::array<std::string_view, full_size>{};

      structure::for_each(
          [offset = 0U]<typename Range>(auto& output) mutable {
            constexpr auto range_names = []<std::size_t... SubIdx>(std::index_sequence<SubIdx...>) {
              return std::array{
                  std::string_view{enum_name<T, to_underlying<T, Kind>(Range::template get<SubIdx>)>.data()}...};
            }(std::make_index_sequence<Range::size>{});

            std::copy(std::begin(range_names), std::end(range_names), std::next(std::begin(output), offset));
            offset += Range::size;
          },
          buffer);

      return buffer;
    }
  }

public:
  static constexpr auto names = get_enum_names();

  template <std::size_t Idx = 0>
  constexpr static std::string_view search_name(underlying value)
    requires(Kind == EnumKind::Linear)
  {
    using range = structure::template get<Idx>;

    if (range::contains(value)) {
      auto offset = value - range::min;
      // TODO assert offset is >= 0
      return names[std::size_t(offset)];
    }

    if constexpr (Idx + 1 < structure::size) {
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
    using range = structure::template get<0>;

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

  constexpr static std::string_view get_name(T value) {
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
}  // namespace librepr::ctei
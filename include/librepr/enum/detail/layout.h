#pragma once
#include <librepr/util/type_list.h>

#include "range.h"
#include "util.h"

namespace librepr::ctei {

template <typename... Values>
struct Layout;

template <>
struct Layout<> {
  using list                      = TypeList<>;
  static constexpr auto full_size = 0;
  static constexpr auto size      = 0;

  template <auto Index>
  using add = Layout<Range<Index>>;

  template <typename T, auto V, auto Idx = V>
  using try_one = std::conditional_t<is_enum_value<T, V>(), add<Idx>, Layout>;

  constexpr static bool is_flag_like = true;
};

template <typename... Ranges>
struct Layout {
  using list                       = TypeList<Ranges...>;
  using last                       = list::template get<list::size - 1>;
  static constexpr auto next_index = ((Ranges::max + 1), ...);
  static constexpr auto full_size  = ((Ranges::size) + ...);
  static constexpr auto size       = sizeof...(Ranges);

  template <auto Idx>
  using add = std::conditional_t<
      next_index == Idx,
      rebox<typename list::template slice<0, list::size - 1>::template append<typename last::expand>, Layout>,
      Layout<Ranges..., Range<Idx>>>;

  template <typename T, auto V, auto Idx = V>
  using try_one = std::conditional_t<is_enum_value<T, V>(), add<Idx>, Layout>;

  constexpr static bool is_flag_like = (Ranges::is_binary_powers() && ...);
};

}  // namespace librepr::ctei
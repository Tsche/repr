#pragma once
#include <type_traits>
#include <librepr/util/collections/list.h>

#include "range.h"
#include "util.h"

namespace librepr::ctei {

template <typename... Ranges>
struct RangeList : TypeListBase<RangeList, Ranges...>{
  using last                       = typename RangeList::template get<0>;
  constexpr static auto next_index = last::max + 1;

  template <auto Idx>
  using add = std::conditional_t<
      next_index == Idx,
      typename RangeList::template tail<0>::template prepend<typename last::template expand<1>>,
      typename RangeList::template prepend<Range<Idx>>
  >;

  template <typename T>
  using add_range = std::conditional_t<
      next_index == T::min,
      typename RangeList::template tail<0>::template prepend<typename last::template expand<T::size>>,
      typename RangeList::template prepend<T>
  >;

  template <typename T, auto V, auto Idx = V>
  using try_one = std::conditional_t<is_enum_value<T, V>(), add<Idx>, RangeList>;

  constexpr static bool is_binary_powers(){
    return (Ranges::is_binary_powers() && ...);
  }
};

template <>
struct RangeList<> : TypeListBase<RangeList> {
  constexpr static auto full_size = 0;

  template <auto Index>
  using add = RangeList<Range<Index>>;

  template <typename T>
  using add_range = RangeList<T>;

  template <typename T, auto V, auto Idx = V>
  using try_one = std::conditional_t<is_enum_value<T, V>(), add<Idx>, RangeList>;

  constexpr static bool is_binary_powers(){
    // special case empty range list to detect flags outside of initial search range
    return true;
  }
};
}  // namespace librepr::ctei
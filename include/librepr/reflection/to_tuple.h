#pragma once
#include <tuple>

#include "arity.h"

namespace librepr::detail {

template <typename T>
auto to_tuple(T const& object) {
  //TODO generate this using cursed macros

  constexpr static int member_count = arity<std::decay_t<T>>();
  if constexpr (member_count == 0) {
    return std::tie();
  } else if constexpr (member_count == 1) {
    auto& [member_0] = object;
    return std::tie(member_0);
  } else if constexpr (member_count == 2) {
    auto& [member_0, member_1] = object;
    return std::tie(member_0, member_1);
  } else if constexpr (member_count == 3) {
    auto& [member_0, member_1, member_2] = object;
    return std::tie(member_0, member_1, member_2);
  } else if constexpr (member_count == 4) {
    auto& [member_0, member_1, member_2, member_3] = object;
    return std::tie(member_0, member_1, member_2, member_3);
  } else if constexpr (member_count == 5) {
    auto& [member_0, member_1, member_2, member_3, member_4] = object;
    return std::tie(member_0, member_1, member_2, member_3, member_4);
  }
}
}  // namespace librepr
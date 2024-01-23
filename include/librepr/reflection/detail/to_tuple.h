#pragma once
#include <memory>
#include <type_traits>
#include <tuple>

#include "visit_aggregate.h"

namespace librepr::detail {

template <typename T>
  requires(std::is_aggregate_v<std::remove_cvref_t<T>> && !std::is_array_v<std::remove_cvref_t<T>>)
constexpr auto to_tuple(T&& object) {
  return visit_aggregate([]<typename... Ts>(Ts&&... members){
    return std::tie(std::forward<Ts>(members)...);
  }, std::forward<T>(object));
}
}
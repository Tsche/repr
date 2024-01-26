#pragma once
#include <memory>
#include <type_traits>
#include <utility>

#include <librepr/util/reftuple.h>
#include "visit_aggregate.h"

namespace librepr::detail {
template <typename T>
  requires(std::is_aggregate_v<std::remove_cvref_t<T>> && !std::is_array_v<std::remove_cvref_t<T>>)
auto to_reftuple(T&& object){
  return visit_aggregate(make_reftuple, std::forward<T>(object));
}
}
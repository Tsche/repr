#pragma once
#include <type_traits>

namespace librepr::detail {
struct Universal {
  template <typename T>
  [[implicit]] operator T() {}
};

template <typename T>
requires (std::is_aggregate_v<T> && !std::is_array_v<T>)
consteval auto arity(auto... parameters) {
  if constexpr (requires { T{parameters..., Universal{}}; }) {
    return arity<T>(parameters..., Universal{});
  } else {
    return sizeof...(parameters);
  }
}

}  // namespace librepr
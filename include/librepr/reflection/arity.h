#pragma once

namespace librepr::detail {
struct Universal {
  template <typename T>
  [[implicit]] operator T() {}
};

template <typename T>
consteval auto arity(auto... parameters) {
  if constexpr (requires { T{parameters...}; }) {
    return arity<T>(parameters..., Universal{});
  } else {
    return sizeof...(parameters) - 1;
  }
}

}  // namespace librepr
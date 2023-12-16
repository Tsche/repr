#pragma once

namespace librepr {

template <typename T>
struct Wrap {
  T value;
};

template <typename T>
Wrap(T) -> Wrap<T>;

template <auto V, typename T = decltype(V)>
struct Constant {
  using value_type            = T;
  static constexpr auto value = V;
};

template <auto V>
inline constexpr Constant<V> constant{};

}  // namespace librepr
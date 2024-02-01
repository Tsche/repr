#pragma once
#include <utility>

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
  constexpr static auto value = V;
};

template <auto V>
inline constexpr Constant<V> constant{};

namespace detail {
template <typename T>
extern const Wrap<T> fake_obj;

template <class U>
constexpr U& convert_ref(U& ref) noexcept {
  // https://cplusplus.github.io/LWG/issue2993
  return ref;
}

template <class U>
void convert_ref(U&&) = delete;

template <typename From, typename To>
concept ref_convertible_to = requires { convert_ref<To>(std::declval<From>()); };

}

}  // namespace librepr
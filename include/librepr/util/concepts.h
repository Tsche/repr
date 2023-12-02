#pragma once
#include <concepts>
#include <type_traits>
#include <string>

namespace librepr::detail {
template <typename T>
concept pair_like = requires(T t) {
  t.first;
  t.second;
  requires std::is_same_v<typename T::first_type, decltype(t.first)>;
  requires std::is_same_v<typename T::second_type, decltype(t.second)>;
};

template <typename T>
concept has_repr_member = requires(T const& obj) {
  { obj.repr() } -> std::same_as<std::string>;
};

template <typename T>
concept is_scoped_enum = !std::is_convertible_v<T, std::underlying_type_t<T>>;
}
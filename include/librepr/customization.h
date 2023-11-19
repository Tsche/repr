#pragma once
#include <type_traits>
#include <concepts>

namespace librepr {
template <typename T>
struct EnumSettings;

template <typename T>
concept has_search_range = std::is_enum_v<T> && requires(T obj) {
  { EnumSettings<T>::search_range::min };
  { EnumSettings<T>::search_range::max };
};

template <typename T>
concept has_is_flag = std::is_enum_v<T> && requires(T obj) {
  { EnumSettings<T>::is_flag } -> std::same_as<bool>;
};

template <typename T>
struct Settings;

}  // namespace librepr

#pragma once
#include <concepts>
#include <type_traits>

#include <librepr/util/list.h>
#include <librepr/util/pack.h>

namespace librepr {
// Enum customization traits

template <typename T>
struct Settings;

template <typename T>
concept has_search_range = std::is_enum_v<T> && requires(T obj) {
  { Settings<T>::search_range::min };
  { Settings<T>::search_range::max };
};

template <typename T>
concept has_is_flag = std::is_enum_v<T> && requires(T obj) {
  { Settings<T>::is_flag } -> std::same_as<bool>;
};

template <typename T>
concept has_fast_search = std::is_enum_v<T> && requires(T obj) {
  { Settings<T>::fast_search } -> std::same_as<bool>;
};

template <typename T>
concept has_enumerators = std::is_enum_v<T> && requires(T obj) { typename Settings<T>::enumerators; };

}  // namespace librepr
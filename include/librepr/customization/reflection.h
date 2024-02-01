#pragma once
#include <concepts>
#include <string_view>

// TODO decide whether to constrain has_custom_members to be a value list
// #include <librepr/util/collections/list.h>

namespace librepr {
// Reflection customization traits

template <typename T>
struct Settings;

template <typename T>
concept has_custom_members = std::is_class_v<T> && requires { 
  typename Settings<T>::members;
  //typename pack::rebox_v<T, ValueList>; // can rebox into ValueList -> members is a value list
};

template <typename T>
concept has_is_literal = std::is_enum_v<T> && requires(T obj) {
  { Settings<T>::is_literal } -> std::same_as<bool>;
};

template <typename T>
concept has_name = std::is_enum_v<T> && requires(T obj) {
  { Settings<T>::name } -> std::convertible_to<std::string_view>;
};

}
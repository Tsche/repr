#pragma once
#include <string>
#include <format>
#include <concepts>

#include <librepr/detail/visibility.h>
#include <librepr/object_info.h>

namespace librepr {

template <typename T>
  requires requires(T const obj) {
    { obj.repr() } -> std::same_as<std::string>;
  }
EXPORT ObjectInfo<T, Literal> repr(T const& obj) {
  return {obj, obj.repr()};
}

}  // namespace librepr
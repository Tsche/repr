#pragma once
#include <type_traits>
#include <magic_enum.hpp>

namespace librepr {
  template <typename T>
  requires std::is_enum_v<T>
  std::string repr(T const& obj){
    return std::string{magic_enum::enum_name(obj)};
  }
}
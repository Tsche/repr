#pragma once
#include <string>
#include <format>

namespace librepr {

template <typename T>
std::string repr(T const& obj){
  // Fallback for when we couldn't find a suitable repr
  return std::format(" object at {:p}", static_cast<void const*>(&obj));
}

}
#pragma once

#include <librepr/repr/fundamental.h>
#include <librepr/repr/pointer.h>
#include <librepr/repr/enum.h>

namespace librepr {

template <typename T>
std::string repr(T const& obj){
  // Fallback for when we couldn't find a suitable repr
  return std::format(" object at {:p}", static_cast<void const*>(&obj));
}

}
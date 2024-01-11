#pragma once
#include <string>
#include <librepr/detail/format.h>

namespace librepr {

template <typename T>
std::string repr(T const& obj){
  // Fallback for when we couldn't find a suitable repr
  return REPR_FORMAT(" object at {:p}", static_cast<void const*>(&obj));
}

}
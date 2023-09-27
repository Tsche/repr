#pragma once
#include "fundamental.h"
#include "pointer.h"
#include <librepr/reflection/name.h>

namespace librepr {

template <typename T>
std::string get_name();

template <typename T>
std::string repr(T const& obj){
  // Fallback for when we couldn't find a suitable repr
  return std::format("[{} object at {:p}]", librepr::get_name<T>(), static_cast<void const*>(&obj));
}
}
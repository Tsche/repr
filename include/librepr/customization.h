#pragma once
#include <type_traits>

namespace librepr {
  template <typename T>
    requires std::is_enum_v<T>
  struct EnumSettings;


}
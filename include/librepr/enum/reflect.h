#pragma once
#include <type_traits>
#include <string_view>

#include <librepr/detail/default.h>

#if USING(REPR_MAGIC_ENUM)
  #if __has_include(<magic_enum.hpp>)
    #include <magic_enum.hpp>
  #elif __has_include(<magic_enum/magic_enum.hpp>)
    // compiler explorer uses this include name
    #include <magic_enum/magic_enum.hpp>
  #else
    #error "Configured to use magic_enum but could not find magic_enum"
  #endif
#else
  #include "search.h"
#endif

namespace librepr {

  template <typename T>
    requires std::is_enum_v<T> 
  [[nodiscard]] constexpr auto enum_names() noexcept{
#if USING(REPR_MAGIC_ENUM)
    return magic_enum::enum_names<T>();
#else
    return ctei::dump_enum<T>::get_names();
#endif
  }

  template <typename T>
    requires std::is_enum_v<T>
  [[nodiscard]] constexpr auto enum_name(T value) noexcept{
#if USING(REPR_MAGIC_ENUM)
    return magic_enum::enum_name(value);
#else
    return ctei::dump_enum<T>::get_name(value);
#endif
  }

  template <auto V>
    requires std::is_enum_v<decltype(V)>
  [[nodiscard]] constexpr std::string_view enum_name() noexcept{
#if USING(REPR_MAGIC_ENUM)
    return magic_enum::enum_name<V>();
#else
    // TODO
    return ctei::enum_name<decltype(V), V>.data();
#endif
  }

}
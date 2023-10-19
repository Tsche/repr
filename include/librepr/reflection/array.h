#pragma once
#include <format>
#include <string>
#include <sstream>
#include <type_traits>

#include <librepr/reflection/name.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T, std::size_t N>
struct Reflect<T[N]> {
  using type = T;

  static std::string dump(T const (&obj)[N], bool /*with_type*/ = false, bool explicit_types = false) {
    if constexpr (std::is_same_v<T, char>) {
      // emit as string literal
      return librepr::repr(obj);
    } else {
      std::ostringstream list{};

      list << '{';
      for (std::size_t idx = 0; idx < N; ++idx) {
        if (idx != 0) {
          list << ", ";
        }

        list << Reflect<type>::dump(obj[idx], explicit_types, explicit_types);
      }
      list << '}';
      return list.str();
    }
  }

  static std::string layout() { return std::format("{}[{}]", Reflect<type>::layout(), N); }
};

template <typename T>
struct Reflect<T[]> {  // NOLINT
  using type = T;

  static std::string dump(T const* /*obj*/, bool /*with_type*/ = false, bool /*explicit_types*/ = false) {
    return "{}";
  }

  static std::string layout() { return std::format("[{}]", Reflect<type>::layout()); }
};

}  // namespace librepr
#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <format>

#include <librepr/type/name.h>
#include <librepr/visitors/visitor.h>
#include <librepr/util/concepts.h>

#include <librepr/macro/default.h>
#include <librepr/enum/reflect.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
  requires std::is_enum_v<T>
struct Reflect<T> {
  using type = T;

  template <typename V>
  static void visit(V&& visitor, type const& obj) {
    Visit::value(visitor, obj);
  }

  static std::string layout() {
    std::ostringstream list{};
    auto values = librepr::enum_names<T>();
    for (auto const& element : values) {
      if (&element != &*std::begin(values)) {
        list << " | ";
      }
      #if USING(REPR_USE_MAGIC_ENUM)
      if constexpr (detail::is_scoped_enum<T>) {
        list << librepr::get_name<T>();
        list << "::";
      }
      #endif
      list << element;
    }
    return list.str();
  }
};

}  // namespace librepr
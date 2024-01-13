#pragma once
#include <string>
#include <type_traits>

#include <librepr/detail/format.h>
#include <librepr/options.h>
#include <librepr/util/concepts.h>
#include <librepr/name/type.h>
#include <librepr/visitors/visitor.h>

namespace librepr {
template <typename T>
struct Reflect;

template <detail::pair_like T>
struct Reflect<T> {
  using type        = T;
  using first_type  = std::remove_cv_t<std::remove_reference_t<typename T::first_type>>;
  using second_type = std::remove_cv_t<std::remove_reference_t<typename T::second_type>>;

  template <typename V>
  static void visit(V&& visitor, T const& obj) {
    Visit::type<T>(visitor);
    ScopeGuard guard{visitor};

    Reflect<first_type>::visit(visitor, obj.first);
    Reflect<second_type>::visit(visitor, obj.second);
  }

  static std::string layout() {
    return REPR_FORMAT("{{{}, {}}}", Reflect<first_type>::layout(), Reflect<second_type>::layout());
  }
};

}  // namespace librepr
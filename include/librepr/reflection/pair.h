#pragma once
#include <format>
#include <string>
#include <type_traits>

#include <librepr/options.h>
#include <librepr/util/concepts.h>
#include <librepr/type/name.h>
#include <librepr/visitors/visitor.h>

namespace librepr {
template <typename T>
struct Reflect;

template <detail::pair_like T>
struct Reflect<T> {
  using type        = T;
  using first_type  = std::remove_cv_t<std::remove_reference_t<typename T::first_type>>;
  using second_type = std::remove_cv_t<std::remove_reference_t<typename T::second_type>>;

  static void visit(Visitor::Values auto&& visitor, type const& obj) {
    ScopeGuard guard{visitor, std::type_identity<type>{}};
    Reflect<first_type>::visit(std::forward<decltype(visitor)>(visitor), obj.first);
    Reflect<second_type>::visit(std::forward<decltype(visitor)>(visitor), obj.second);
  }

  static std::string layout() {
    return std::format("{{{}, {}}}", Reflect<first_type>::layout(), Reflect<second_type>::layout());
  }
};

}  // namespace librepr
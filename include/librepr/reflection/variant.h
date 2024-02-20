#pragma once
#include <concepts>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

#include <librepr/name/type.h>

#include <librepr/util/collections/list.h>
#include <librepr/util/overload.h>

#include "category.h"

namespace librepr {
template <typename T>
struct Reflect;

namespace detail {
template <typename T>
struct VariantDetector {
  static constexpr bool value = false;
};

template <template <typename...> class Variant, typename... Ts>
  requires std::derived_from<Variant<Ts...>, std::variant<Ts...>>
struct VariantDetector<Variant<Ts...> const> {
  static constexpr bool value = true;
  using alternatives          = typename TypeList<Ts...>::template map<std::add_const_t>;
};

template <template <typename...> class Variant, typename... Ts>
  requires std::derived_from<Variant<Ts...>, std::variant<Ts...>>
struct VariantDetector<Variant<Ts...>> {
  static constexpr bool value = true;
  using alternatives          = TypeList<Ts...>;
};

}  // namespace detail

template <typename T>
concept is_variant = detail::VariantDetector<T>::value;

template <is_variant T>
struct Reflect<T> : category::Type<T> {
  using type                        = T;
  using alternatives                = typename detail::VariantDetector<T>::alternatives;
  constexpr static bool can_descend = true;

  template <typename V>
  static void visit(V&& visitor, type& obj) {
    auto overload_set = alternatives::invoke([&visitor]<typename... Ts>() {
      return detail::Overload{[&visitor](Ts& alternative) { visitor(category::Value<Reflect<Ts>>{alternative}); }...};
    });

    std::visit(overload_set, obj);
  }

  template <typename V>
  static void visit(V&& visitor) {
    alternatives::for_each([&visitor]<typename U>(){
      visitor(Reflect<U>{});
    });
  }
};

}  // namespace librepr
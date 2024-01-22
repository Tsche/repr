#pragma once
#include <concepts>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

#include <librepr/name/type.h>

#include <librepr/util/list.h>
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
  using alternatives          = TypeList<Ts...>::template map<std::add_const_t>;
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
  using alternatives                = detail::VariantDetector<T>::alternatives;
  constexpr static bool can_descend = true;

  template <typename V>
  static void visit(V&& visitor, type& obj) {
    auto overload_set = alternatives::invoke([&visitor]<typename... Ts>() {
      return detail::Overload{[&visitor](Ts& alternative) { visitor(category::Value<Reflect<Ts>>{alternative}); }...};
    });

    std::visit(overload_set, obj);
  }

  /*static std::string layout() {
    auto output = std::string("<");
    TypeList<Ts...>::enumerate([&output]<typename Member>(std::size_t index) {
      if (index != 0) {
        output += " | ";
      }

      output += Reflect<Member>::layout();
    });

    return output + '>';
  }*/
};

template <template <typename...> class Variant, typename... Ts>
  requires std::derived_from<Variant<Ts...>, std::variant<Ts...>>
struct Reflect<Variant<Ts...>> : category::Type<Variant<Ts...>> {
  using type                        = Variant<Ts...>;
  constexpr static bool can_descend = true;

  template <typename V>
  static void visit(V&& visitor, type& obj) {
    std::visit(detail::Overload{[&visitor](Ts& alternative) { visitor(category::Value{alternative}); }...}, obj);
  }
};
}  // namespace librepr
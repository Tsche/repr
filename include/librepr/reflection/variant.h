#pragma once
#include <concepts>
#include <sstream>
#include <string>

#include <librepr/name/type.h>

#include <librepr/util/list.h>
#include <librepr/util/overload.h>

#include <librepr/visitors/visitor.h>

namespace librepr {
template <typename T> struct Reflect;

template <template <typename...> class Variant, typename... Ts>
  requires std::derived_from<Variant<Ts...>, std::variant<Ts...>>
struct Reflect<Variant<Ts...>> {
  using type = Variant<Ts...>;

  template <typename V> static void visit(V &&visitor, type const &obj) {
    Visit::type<type>(visitor);
    ScopeGuard guard{visitor};

    std::visit(detail::Overload{[&visitor](Ts const &alternative) {
                 return Reflect<Ts>::visit(visitor, alternative);
               }...},
               obj);
  }

  template <typename V>
  static void visit(V&& visitor) {
    Visit::type<type>(visitor);
    ScopeGuard guard{visitor};
    (Visit::type<Ts>(visitor), ...);
  }

  static std::string layout() {
    auto output = std::string("<");
    TypeList<Ts...>::enumerate([&output]<typename Member>(std::size_t index) {
      if (index != 0) {
        output += " | ";
      }

      output += Reflect<Member>::layout();
    });

    return output + '>';
  }
};
} // namespace librepr
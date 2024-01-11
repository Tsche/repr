#pragma once
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>
#include <concepts>


#include <librepr/type/name.h>

#include <librepr/util/overload.h>
#include <librepr/util/list.h>

#include <librepr/visitors/visitor.h>

namespace librepr {
template <typename T>
struct Reflect;


template <template <typename...> class Variant, typename... Ts>
  requires std::derived_from<Variant<Ts...>, std::variant<Ts...>>
struct Reflect<Variant<Ts...>> {
  using type = Variant<Ts...>;

  template <Visitor::Values V>
  static void visit(V&& visitor, auto const& obj) {
    ScopeGuard guard{visitor, std::type_identity<type>{}};
    std::visit(detail::Overload{[&visitor](Ts const& alternative) {
                                    return Reflect<Ts>::visit(std::forward<V>(visitor), alternative);
                                  }...}, obj);
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
}  // namespace librepr
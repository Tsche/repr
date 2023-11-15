#pragma once
#include <sstream>
#include <string>
#include <type_traits>

#include <librepr/type/name.h>

#include <librepr/util/concepts.h>
#include <librepr/util/overload.h>

#include <librepr/visitors/visitor.h>


namespace librepr {
template <typename T>
struct Reflect;

template <template <typename...> class Variant, typename... Ts>
  requires detail::variant_like<Variant<Ts...>>
struct Reflect<Variant<Ts...>> {
  using type = Variant<Ts...>;

  template <Visitor::Values V>
  static void visit(V&& visitor, type const& obj) {
    ScopeGuard guard{visitor, std::type_identity<type>{}};
    std::visit(detail::Overload{[&visitor](Ts const& alternative) {
                                    return Reflect<Ts>::visit(std::forward<decltype(visitor)>(visitor), alternative);
                                  }...}, obj);
  }

  static std::string layout() {
    std::ostringstream list{};
    list << '<';

    const char* sep = "";
    (((list << sep << librepr::get_name<Ts>()), sep = " | "), ...);

    list << '>';
    return list.str();
  }
};
}  // namespace librepr
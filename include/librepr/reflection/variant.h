#pragma once
#include <sstream>
#include <string>
#include <format>
#include <type_traits>

#include <librepr/reflection/name.h>

#include <librepr/detail/concepts.h>
#include <librepr/detail/overload.h>

#include "visitor.h"


namespace librepr {
template <typename T>
struct Reflect;

template <template <typename...> class Variant, typename... Ts>
  requires detail::variant_like<Variant<Ts...>>
struct Reflect<Variant<Ts...>> {
  using type = Variant<Ts...>;

  static void visit(auto&& visitor, type const& obj) {
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
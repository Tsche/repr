#pragma once
#include <sstream>
#include <string>
#include <format>
#include <type_traits>

#include <librepr/detail/concepts.h>
#include <librepr/detail/overload.h>
#include <librepr/reflection/name.h>

namespace librepr {
template <typename T>
struct Reflect;

template <template <typename...> class Variant, typename... Ts>
  requires detail::variant_like<Variant<Ts...>>
struct Reflect<Variant<Ts...>> {
  using type = Variant<Ts...>;

  static std::string dump(type const& obj, bool with_type = false, bool explicit_types = false) {
    auto stringified = std::visit(detail::Overload{[explicit_types](Ts const& alternative) {
                                    return Reflect<Ts>::dump(alternative, explicit_types, explicit_types);
                                  }...},
                                  obj);

    return std::format("{}{{{}}}", with_type ? librepr::get_name<Variant<Ts...>>() : "", stringified);
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
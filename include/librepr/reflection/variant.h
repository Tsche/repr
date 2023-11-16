#pragma once
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

#include <librepr/type/name.h>

#include <librepr/util/concepts.h>
#include <librepr/util/overload.h>

#include <librepr/visitors/visitor.h>

namespace librepr {
template <typename T>
struct Reflect;

namespace detail {
  /// Stupid fucking bullshit hack to get Reflect to
  /// stop matching random ass types with Variant<...>
  struct GenericVisitor {
    void operator()(auto&&) { }
    static auto X(auto&& v) // NOLINT
      -> decltype(std::visit(
        std::declval<GenericVisitor&>(), 
        std::forward<decltype(v)>(v))) { }
  };

  template <typename T>
  concept is_variant_like = variant_like<T> &&
    requires(T t) { GenericVisitor::X(t); };
}  // namespace detail

template <template <typename...> class Variant, typename... Ts>
  requires detail::is_variant_like<Variant<Ts...>>
struct Reflect<Variant<Ts...>> {
  using type = Variant<Ts...>;

  template <Visitor::Values V>
  static void visit(V&& visitor, type const& obj) {
    ScopeGuard guard{visitor, std::type_identity<type>{}};
    detail::Overload ov_set {[&visitor](Ts const& alternative) -> void {
      return Reflect<Ts>::visit(std::forward<decltype(visitor)>(visitor), alternative);
    }...};
    std::visit(std::move(ov_set), obj);
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
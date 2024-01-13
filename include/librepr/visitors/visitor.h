#pragma once

#include <concepts>
#include <librepr/macro/util.h>
#include <librepr/util/wrap.h>
#include <string_view>

namespace librepr {
/*
// TODO
namespace Visitor {
template <typename V>
concept Values = requires(V visitor) {
  // value visitation
  { visitor.template operator()<int>(int{}) };
};

template <typename V>
concept Types = requires(V visitor) {
  // type visitation
  { visitor.template operator()<int>() };
};

template <typename V>
concept Hierarchical = requires(V visitor) {
  // nesting control
  { visitor.increase_nesting() };
  { visitor.decrease_nesting() };
} && Values<V> && Types<V>;
}  // namespace Visitor
*/

enum class VisitEvent { type, value, nesting, member_name };

template <VisitEvent E>
struct Visitor {
  constexpr static auto value = E;
};

struct Visit {
  template <typename T>
  static auto value(auto&& visitor, T const& obj) {
    LIBREPR_MAYBE_DO(visitor.value(obj))
    else LIBREPR_MAYBE_DO(visitor.template operator()<T>(obj))
  }

  template <typename T>
  static auto type(auto&& visitor) {
    LIBREPR_MAYBE_DO(visitor.template type<T>())
    else LIBREPR_MAYBE_DO(visitor.template operator()<T>())
  }

  static auto nesting(auto&& visitor, bool increase) {
    LIBREPR_MAYBE_DO(visitor.nesting(increase))
    else LIBREPR_MAYBE_DO(visitor(constant<VisitEvent::nesting>, increase))
  }
  static auto member_name(auto&& visitor, std::string_view name) {
    LIBREPR_MAYBE_DO(visitor.member_name(name))
    else LIBREPR_MAYBE_DO(visitor(constant<VisitEvent::member_name>, name))
  }
};

template <template <typename> class Tag, typename T>
concept TagType = std::same_as<typename Tag<T>::type, T>;

template <typename V>
struct [[nodiscard]] ScopeGuard {
  ScopeGuard(const ScopeGuard&)            = delete;
  ScopeGuard(ScopeGuard&&)                 = delete;
  ScopeGuard& operator=(const ScopeGuard&) = delete;
  ScopeGuard& operator=(ScopeGuard&&)      = delete;

  explicit ScopeGuard(V& visitor_) : visitor(visitor_) { 
    Visit::nesting(visitor, true);
  }

  ~ScopeGuard() { Visit::nesting(visitor, false); }

  // ScopeGuard is not moveable or copyable. Clang-tidy should not diagnose
  // cppcoreguidelines-avoid-const-or-ref-data-members here but does anyway..
  // wtf?
  V& visitor;  // NOLINT
};

}  // namespace librepr
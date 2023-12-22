#pragma once

#include <concepts>
#include <string_view>
#include <librepr/macro/util.h>

namespace librepr {
/*
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

#define LIBREPR_GENERATE_DISPATCH_BRANCH(variant) \
  if constexpr (requires {                        \
                  { visitor variant };            \
                }) {                              \
    visitor variant;                              \
  }

#define LIBREPR_GENERATE_DISPATCH(name, args, member, ...)       \
  static auto name(auto&& visitor LIBREPR_MAYBE_COMMA(args)) {   \
    LIBREPR_GENERATE_DISPATCH_BRANCH(member)                     \
    FOR_EACH(else LIBREPR_GENERATE_DISPATCH_BRANCH, __VA_ARGS__) \
  }

struct Visit {
  template <typename T>
  LIBREPR_GENERATE_DISPATCH(value, T const& value, 
      .value(value),
      .template operator()<T>(value)
  )

  template <typename T>
  LIBREPR_GENERATE_DISPATCH(type,, 
      .template type<T>(),
      .template operator()<T>()
  )

  LIBREPR_GENERATE_DISPATCH(nesting, bool increase, 
      .nesting(increase),
      (Visitor<VisitEvent::nesting>{}, increase)
  )

  LIBREPR_GENERATE_DISPATCH(member_name, std::string_view name, 
      .member_name(name),
      (Visitor<VisitEvent::member_name>{}, name)
  )
};

#undef LIBREPR_GENERATE_DISPATCH_BRANCH
#undef LIBREPR_GENERATE_DISPATCH

template <template <typename> class Tag, typename T>
concept TagType = std::same_as<typename Tag<T>::type, T>;

template <typename V, typename T>
struct [[nodiscard]] ScopeGuard {
  ScopeGuard(const ScopeGuard&)            = delete;
  ScopeGuard(ScopeGuard&&)                 = delete;
  ScopeGuard& operator=(const ScopeGuard&) = delete;
  ScopeGuard& operator=(ScopeGuard&&)      = delete;

  template <template <typename> class Tag>
    requires TagType<Tag, T>
  ScopeGuard(V& visitor_, Tag<T>) : ScopeGuard(visitor_) {}

  explicit ScopeGuard(V& visitor_) : visitor(visitor_) {
    Visit::type<T>(visitor);
    Visit::nesting(visitor, true);
  }

  ~ScopeGuard() {
    Visit::nesting(visitor, false);
  }

  V& visitor;
};

}  // namespace librepr
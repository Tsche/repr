#pragma once

#include <concepts>

namespace librepr {

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

template <template <typename> class Tag, typename T>
concept TagType = std::same_as<typename Tag<T>::type, T>;

template <typename V, typename T>
struct ScopeGuard {
  ScopeGuard(const ScopeGuard&)            = delete;
  ScopeGuard(ScopeGuard&&)                 = delete;
  ScopeGuard& operator=(const ScopeGuard&) = delete;
  ScopeGuard& operator=(ScopeGuard&&)      = delete;

  template <template <typename> class Tag>
    requires TagType<Tag, T>
  ScopeGuard(V& visitor_, Tag<T>) : ScopeGuard(visitor_) {}

  explicit ScopeGuard(V& visitor_) : visitor(visitor_) {
    if constexpr (Visitor::Types<V>) {
      visitor.template operator()<T>();
    }

    if constexpr (Visitor::Hierarchical<V>){
      visitor.increase_nesting();
    }
  }

  ~ScopeGuard()
    requires Visitor::Hierarchical<V>
  {
    visitor.decrease_nesting();
  }

  ~ScopeGuard() = default;

  V& visitor;
};

}  // namespace librepr
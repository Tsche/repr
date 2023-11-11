#pragma once

#include <concepts>

template <typename V>
concept ObjectVisitor = requires(V visitor) {
  { visitor.template operator()<int>() };       // can visit types
  { visitor.template operator()<int>(int{}) };  // can visit values

  // nesting control
  { visitor.increase_nesting() };
  { visitor.decrease_nesting() };
};

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
    if constexpr (ObjectVisitor<V>) {
      visitor.template operator()<T>();
      visitor.increase_nesting();
    }
  }

  ~ScopeGuard()
    requires ObjectVisitor<V>
  {
    visitor.decrease_nesting();
  }

  ~ScopeGuard() = default;

  V& visitor;
};

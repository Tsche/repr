#pragma once
#include <concepts>
#include <cstddef>
#include <string_view>
#include <type_traits>
#include <utility>
#include <librepr/macro/util.h>

#include <librepr/name/type.h>
#include <librepr/util/util.h>

namespace librepr {
template <typename T>
struct Reflect;
}

namespace librepr::category {

template <typename T>
concept has_name = requires(T obj) { std::string_view{obj.name()}; };

template <typename T>
concept has_parent = requires { typename T::parent; };

template <typename T>
concept has_members = requires { typename T::members; };

template <typename T>
concept has_extent = requires {
  { T::extent } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept has_alternatives = requires { typename T::alternatives; };

template <typename T>
concept has_enumerator_names = requires { T::enumerator_names; };

template <typename T>
concept has_value = requires(T obj) { obj.value(); };

template <typename T>
concept can_descend = has_members<T> || has_extent<T> || has_alternatives<T> || T::can_descend;

template <typename T>
concept is_iterable = T::is_iterable;

template <class T>
struct Value : T {
  // essentially a std::reference_wrapper without operator()
  using descend = T;
  using type    = typename T::type;

  template <detail::ref_convertible_to<type> U>
    requires(!std::same_as<Value, std::remove_cvref_t<U>>)
  constexpr explicit(false) Value(U&& obj) noexcept(noexcept(detail::convert_ref<type>(std::forward<U>(obj))))
      : data(std::addressof(detail::convert_ref<type>(std::forward<U>(obj)))) {}

  Value(const Value&) noexcept            = default;
  Value& operator=(const Value&) noexcept = default;
  ~Value()                                = default;

  constexpr explicit(false) operator type&() const noexcept { return *data; }
  constexpr type& operator*() const noexcept { return *data; }
  constexpr type* operator->() const noexcept { return data; }

  [[nodiscard]] constexpr type& value() const noexcept { return *data; }

  template <typename V>
  void visit(V&& visitor) {
    LIBREPR_MAYBE_DO(T::visit(std::forward<V>(visitor), value()));
  }

private:
  type* data;
};

template <class T>
Value(T&) -> Value<Reflect<T>>;

template <typename T>
struct Type {
  using type = T;
  [[nodiscard]] static auto type_name() { return librepr::get_name<type>(); }

  template <typename V>
  static void visit(V&& visitor) {
    LIBREPR_MAYBE_DO(Reflect<T>::visit(std::forward<V>(visitor)));
  }
};

}  // namespace librepr::category
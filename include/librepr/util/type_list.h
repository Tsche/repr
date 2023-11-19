#pragma once
#include <tuple>

#include <librepr/detail/macros.h>

namespace librepr {

namespace detail {
#if __has_builtin(__type_pack_element)
template <std::size_t index, typename... Ts>
struct GetImpl {
  using type = __type_pack_element<index, Ts...>;
};
#else
template <std::size_t index, typename... Ts>
struct GetImpl {};

template <typename T, typename... Ts>
struct GetImpl<0, T, Ts...> {
  using type = T;
};

template <std::size_t index, typename T, typename... Ts>
struct GetImpl<index, T, Ts...> {
  using type = typename GetImpl<index - 1, Ts...>::type;
};
#endif

template<typename, template <typename...> class>
struct ReBoxImpl;

template<template <typename...> class From, typename... Args, template <typename...> class To>
struct ReBoxImpl<From<Args...>, To>{
    using type = To<Args...>;
};
}

template <typename T, template <typename...> class To>
using rebox = typename detail::ReBoxImpl<T, To>::type;

template <typename... Ts>
struct TypeList {
  static constexpr std::size_t size = sizeof...(Ts);

  template <template <typename> class F>
  using map = TypeList<F<Ts>...>;

  template <template <typename> class F>
  using map_t = TypeList<typename F<Ts>::type...>;

  template <typename T>
  using append = TypeList<Ts..., T>;

  template <typename T>
  using prepend = TypeList<T, Ts...>;

  template <std::size_t index>
  requires(sizeof...(Ts) > 0)
  using get = typename detail::GetImpl<index, Ts...>::type;

  template <std::size_t offset, std::size_t... indices>
  static constexpr auto do_slice(std::index_sequence<indices...>) noexcept -> TypeList<get<indices + offset>...>;

  template <std::size_t start, std::size_t end = sizeof...(Ts)>
    requires(end <= sizeof...(Ts))
  using slice = decltype(do_slice<start>(std::make_index_sequence<end - start>{}));

  template <std::size_t index>
  using split = TypeList<slice<0, index>, slice<index>>;

  template <typename T, std::size_t... indices>
  static constexpr auto do_from_tuple(std::index_sequence<indices...>) noexcept
      -> TypeList<std::tuple_element_t<indices, T>...>;

  template <template <typename...> class T>
  using to = T<Ts...>;

  template <typename Tuple>
  using from_tuple = decltype(do_from_tuple<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{}));

  template <typename F, typename... Args>
  static constexpr auto invoke(F&& callable, Args&&... args) {
    return callable.template operator()<Ts...>(std::forward<Args>(args)...);
  }

  template <typename F, typename... Args>
  static constexpr void for_each(F&& callable, Args&&... args) {
    (callable.template operator()<Ts>(std::forward<Args>(args)...), ...);
  }
};

}  // namespace librepr
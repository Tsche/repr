#pragma once
#include <cstddef>
#include <type_traits>
#include <utility>
#include <librepr/util/util.h>
#include "list.h"

namespace librepr {

namespace detail {
template <int Idx, typename T>
struct TupleLeaf {
  T value;
  constexpr T& operator[](Constant<Idx>) { return value; }
};

template <typename... Ts>
struct TupleImpl;

template <int... Idx, typename... Ts>
struct TupleImpl<std::integer_sequence<int, Idx...>, Ts...> : TupleLeaf<Idx, Ts>... {
  using TupleLeaf<Idx, Ts>::operator[]...;
};

template <int V, typename T>
constexpr T& get_impl(TupleLeaf<V, T>& node) noexcept {
  return node.value;
}

template <int V, typename T>
constexpr T const& get_impl(TupleLeaf<V, T> const& node) noexcept {
  return node.value;
}
}  // namespace detail

template <typename... Ts>
struct Tuple : detail::TupleImpl<std::make_integer_sequence<int, sizeof...(Ts)>, Ts...> {};

template <std::size_t Idx, typename... Ts>
constexpr decltype(auto) get(Tuple<Ts...> const& tuple) noexcept {
  return detail::get_impl<Idx>(tuple);
}

template <std::size_t Idx, typename... Ts>
constexpr decltype(auto) get(Tuple<Ts...>& tuple) noexcept {
  return detail::get_impl<Idx>(tuple);
}
}  // namespace librepr

template <std::size_t I, typename... Ts>
struct std::tuple_element<I, librepr::Tuple<Ts...>> {
  using type = typename librepr::TypeList<Ts...>::template get<I>;
};

template <typename... Ts>
struct std::tuple_size<librepr::Tuple<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};
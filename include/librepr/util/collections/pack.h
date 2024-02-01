#pragma once
#include <cstddef>
#include <utility>

#include <librepr/macro/util.h>
#include "pack_generated.h"


namespace librepr::pack {

template <typename T, typename T2>
struct Merge;

template <template <typename...> class List, typename... T1, typename... T2>
struct Merge<List<T1...>, List<T2...>> {
  using type = List<T1..., T2...>;
};

template <std::size_t Idx, typename T>
struct Split;

template <std::size_t Idx, template <typename...> class List, typename... Ts>
  requires(Idx == sizeof...(Ts))
struct Split<Idx, List<Ts...>> {
  using head = List<Ts...>;
  using tail = List<>;
};

template <std::size_t Idx, template <typename...> class List, typename... Ts>
  requires(Idx > MAX_SPEC)
struct Split<Idx, List<Ts...>> {
#if LIBREPR_HAS_BUILTIN(__type_pack_element)
  using type = __type_pack_element<Idx, Ts...>;

  template <std::size_t offset, std::size_t... indices>
  constexpr static auto do_slice(std::index_sequence<indices...>) noexcept
      -> List<__type_pack_element<indices + offset, Ts...>...>;

  using head = decltype(do_slice<0>(std::make_index_sequence<Idx + 1>{}));
  using tail = decltype(do_slice<Idx + 1>(std::make_index_sequence<sizeof...(Ts) - (Idx + 1)>{}));
#else
  using first_page = Split<MAX_SPEC, List<Ts...>>;
  using recurse    = Split<Idx - (MAX_SPEC + 1), typename first_page::tail>;

  using tail = typename recurse::tail;
  using type = typename recurse::type;

  using head = Merge<typename first_page::head, typename recurse::head>::type;
#endif
};

namespace detail {
template <template <auto...> class List, typename T>
struct UnwrapValue;

template <template <auto...> class ValueList, template <typename...> class TypeList, typename... Ts>
struct UnwrapValue<ValueList, TypeList<Ts...>> {
  using type = ValueList<Ts::value...>;
};

template <typename, template <typename...> class>
struct ReBoxImpl;

template <template <typename...> class From, typename... Args, template <typename...> class To>
struct ReBoxImpl<From<Args...>, To> {
  using type = To<Args...>;
};

template <typename, template <auto...> class>
struct ReBoxVImpl;

template <template <auto...> class From, auto... Args, template <auto...> class To>
struct ReBoxVImpl<From<Args...>, To> {
  using type = To<Args...>;
};
}  // namespace detail

template <auto V>
struct ValueWrapper {
  constexpr static decltype(V) value = V;
};

template <template <typename...> class List, auto... Vs>
using wrap = List<ValueWrapper<Vs>...>;

template <template <auto...> class List, typename T>
using unwrap = typename detail::UnwrapValue<List, T>::type;

template <typename T, template <typename...> class To>
using rebox = typename detail::ReBoxImpl<T, To>::type;

template <typename T, template <auto...> class To>
using rebox_v = typename detail::ReBoxVImpl<T, To>::type;

template <std::size_t Idx, typename T>
using get = Split<Idx, T>::type;
}  // namespace librepr::pack
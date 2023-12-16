#pragma once
#include <tuple>
#include <librepr/detail/macros.h>

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

#define MAKE_TYPE(index) T##index,
#define SPECIALIZE_SKIP(index, ...)                                                                         \
  template <template <typename...> class List, FOR_EACH(typename MAKE_TYPE, __VA_ARGS__) typename T##index, \
            typename... Ts>                                                                                 \
  struct Split<index, List<FOR_EACH(MAKE_TYPE, __VA_ARGS__) T##index, Ts...>> {                             \
    using type = T##index;                                                                                  \
    using head = List<FOR_EACH(MAKE_TYPE, __VA_ARGS__) T##index>;                                           \
    using tail = List<Ts...>;                                                                               \
  };

SPECIALIZE_SKIP(0)
SPECIALIZE_SKIP(1, 0)
SPECIALIZE_SKIP(2, 0, 1)
SPECIALIZE_SKIP(3, 0, 1, 2)
SPECIALIZE_SKIP(4, 0, 1, 2, 3)
SPECIALIZE_SKIP(5, 0, 1, 2, 3, 4)
SPECIALIZE_SKIP(6, 0, 1, 2, 3, 4, 5)
SPECIALIZE_SKIP(7, 0, 1, 2, 3, 4, 5, 6)
SPECIALIZE_SKIP(8, 0, 1, 2, 3, 4, 5, 6, 7)
SPECIALIZE_SKIP(9, 0, 1, 2, 3, 4, 5, 6, 7, 8)
SPECIALIZE_SKIP(10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
SPECIALIZE_SKIP(11, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
SPECIALIZE_SKIP(12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11)
SPECIALIZE_SKIP(13, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)
SPECIALIZE_SKIP(14, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13)
SPECIALIZE_SKIP(15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14)
constexpr inline auto MAX_SPEC = 15U;

template <std::size_t Idx, template <typename...> class List, typename... Ts>
  requires(Idx > MAX_SPEC)
struct Split<Idx, List<Ts...>> {
#if __has_builtin(__type_pack_element)
  using type = __type_pack_element<Idx, Ts...>;

  template <std::size_t offset, std::size_t... indices>
  static constexpr auto do_slice(std::index_sequence<indices...>) noexcept
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
  static constexpr decltype(V) value = V;
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
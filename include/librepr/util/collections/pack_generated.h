#pragma once
#include <cstddef>

namespace librepr::pack {

template <std::size_t Idx, typename T>
struct Split;

template <template <typename...> class List, typename T0, typename... Ts>
struct Split<0, List<T0, Ts...>> {
  using type = T0;
  using head = List<T0>;
  using tail = List<Ts...>;
};

template <template <typename...> class List, typename T0, typename T1, typename... Ts>
struct Split<1, List<T0, T1, Ts...>> {
  using type = T1;
  using head = List<T0, T1>;
  using tail = List<Ts...>;
};

template <template <typename...> class List, typename T0, typename T1, typename T2, typename... Ts>
struct Split<2, List<T0, T1, T2, Ts...>> {
  using type = T2;
  using head = List<T0, T1, T2>;
  using tail = List<Ts...>;
};

template <template <typename...> class List, typename T0, typename T1, typename T2, typename T3, typename... Ts>
struct Split<3, List<T0, T1, T2, T3, Ts...>> {
  using type = T3;
  using head = List<T0, T1, T2, T3>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename... Ts>
struct Split<4, List<T0, T1, T2, T3, T4, Ts...>> {
  using type = T4;
  using head = List<T0, T1, T2, T3, T4>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename... Ts>
struct Split<5, List<T0, T1, T2, T3, T4, T5, Ts...>> {
  using type = T5;
  using head = List<T0, T1, T2, T3, T4, T5>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename... Ts>
struct Split<6, List<T0, T1, T2, T3, T4, T5, T6, Ts...>> {
  using type = T6;
  using head = List<T0, T1, T2, T3, T4, T5, T6>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename... Ts>
struct Split<7, List<T0, T1, T2, T3, T4, T5, T6, T7, Ts...>> {
  using type = T7;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename... Ts>
struct Split<8, List<T0, T1, T2, T3, T4, T5, T6, T7, T8, Ts...>> {
  using type = T8;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7, T8>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename... Ts>
struct Split<9, List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ts...>> {
  using type = T9;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename T10,
          typename... Ts>
struct Split<10, List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, Ts...>> {
  using type = T10;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename T10,
          typename T11,
          typename... Ts>
struct Split<11, List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, Ts...>> {
  using type = T11;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename T10,
          typename T11,
          typename T12,
          typename... Ts>
struct Split<12, List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, Ts...>> {
  using type = T12;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename T10,
          typename T11,
          typename T12,
          typename T13,
          typename... Ts>
struct Split<13, List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, Ts...>> {
  using type = T13;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename T10,
          typename T11,
          typename T12,
          typename T13,
          typename T14,
          typename... Ts>
struct Split<14, List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, Ts...>> {
  using type = T14;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>;
  using tail = List<Ts...>;
};

template <template <typename...> class List,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename T10,
          typename T11,
          typename T12,
          typename T13,
          typename T14,
          typename T15,
          typename... Ts>
struct Split<15, List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, Ts...>> {
  using type = T15;
  using head = List<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>;
  using tail = List<Ts...>;
};

constexpr inline auto MAX_SPEC = 15U;

}  // namespace librepr::pack
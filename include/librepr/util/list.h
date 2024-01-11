#pragma once
#include <bits/utility.h>
#include "pack.h"

namespace librepr {

template <template <typename...> class List, typename... Ts>
struct TypeListBase {
private: 
  template <std::size_t Idx>
  using do_split = pack::Split<Idx, List<Ts...>>;

public:
  static constexpr std::size_t size = sizeof...(Ts);

  template <template <typename> class F>
  using map = List<F<Ts>...>;

  template <template <typename> class F>
  using map_t = List<typename F<Ts>::type...>;

  template <std::size_t Idx>
    requires(sizeof...(Ts) > 0 && Idx < sizeof...(Ts))
  using get = pack::Split<Idx, List<Ts...>>::type;

  template <std::size_t Idx>
  using split = List<typename do_split<Idx>::head, typename do_split<Idx>::tail>;

  template <std::size_t Idx>
  using head = do_split<Idx>::head;

  template <std::size_t Idx>
  using tail = do_split<Idx>::tail;

  template <typename T>
  using append = List<Ts..., T>;

  template <typename T>
  using prepend = List<T, Ts...>;

  template <template <typename...> class T>
  using to = T<Ts...>;

  template <typename F, typename... Args>
  static constexpr auto invoke(F&& callable, Args&&... args) {
    return callable.template operator()<Ts...>(std::forward<Args>(args)...);
  }

  template <typename F, typename... Args>
  static constexpr void for_each(F&& callable, Args&&... args) {
    (callable.template operator()<Ts>(std::forward<Args>(args)...), ...);
  }

  template <typename F, typename... Args>
  static constexpr void enumerate(F&& callable, Args&&... args) {
    constexpr auto expects_index_arg = (requires {
      { callable.template operator()<Ts>(std::size_t{}, std::forward<Args>(args)...) };
    } && ...);

    if constexpr (expects_index_arg) {
      auto index = 0U;
      (callable.template operator()<Ts>(index++, std::forward<Args>(args)...), ...);
    } else {
      [&callable, &args...]<std::size_t... Idx>(std::index_sequence<Idx...>) {
        (callable.template operator()<get<Idx>, Idx>(std::forward<Args>(args)...), ...);
      }(std::index_sequence_for<Ts...>{});
    }
  }
};

template <typename... Ts>
struct TypeList : TypeListBase<TypeList, Ts...> {};

template <template <auto...> class List, auto... Vs>
struct ValueList {
  static constexpr std::size_t size = sizeof...(Vs);

  template <template <typename...> class T = TypeList>
  using wrap = pack::wrap<T, Vs...>;

  template <std::size_t Idx>
    requires(sizeof...(Vs) > 0 && Idx < sizeof...(Vs))
  using get = wrap<>::template get<Idx>::type::value;

  template <std::size_t Idx>
  using do_split = pack::Split<Idx, List<Vs...>>;

  template <std::size_t Idx>
  using split = TypeList<pack::unwrap<List, typename do_split<Idx>::head>, 
                         pack::unwrap<List, typename do_split<Idx>::tail>>;

  template <std::size_t Idx>
  using head = do_split<Idx>::head;

  template <std::size_t Idx>
  using tail = do_split<Idx>::tail;

  template <auto V>
  using append = List<Vs..., V>;

  template <auto V>
  using prepend = List<V, Vs...>;

  template <template <auto...> class T>
  using to = T<Vs...>;

  template <typename F, typename... Args>
  static constexpr auto invoke(F&& callable, Args&&... args) {
    return callable.template operator()<Vs...>(std::forward<Args>(args)...);
  }

  template <typename F, typename... Args>
  static constexpr void for_each(F&& callable, Args&&... args) {
    (callable.template operator()<Vs>(std::forward<Args>(args)...), ...);
  }

  template <typename F, typename... Args>
  static constexpr void enumerate(F&& callable, Args&&... args) {
    constexpr auto expects_index_arg = (requires {
      { callable.template operator()<Vs>(std::size_t{}, std::forward<Args>(args)...) };
    } && ...);

    if constexpr (expects_index_arg) {
      auto index = 0U;
      (callable.template operator()<Vs>(index++, std::forward<Args>(args)...), ...);
    } else {
      [&callable, &args...]<std::size_t... Idx>(std::index_sequence<Idx...>) {
        (callable.template operator()<get<Idx>, Idx>(std::forward<Args>(args)...), ...);
      }(std::make_index_sequence<sizeof...(Vs)>{});
    }
  }
};

}  // namespace librepr
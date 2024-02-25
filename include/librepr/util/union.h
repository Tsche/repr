#pragma once
#include "concepts.h"
#include <librepr/macro/warning.h>
namespace librepr {

template <typename T>
struct Settings;

namespace util {

template <typename T>
consteval auto get_tag(){
  if constexpr (requires { T::tag; }){
    return T::tag;
  }
  else if constexpr (requires {Settings<T>::tag; }){
    return Settings<T>::tag;
  }
  else if constexpr (requires (T obj) { {librepr_settings(obj)}; }) {
    return decltype(librepr_settings(T{}))::tag;
  }
  else {
    throw;
  }
}

LIBREPR_WARNING_PUSH
LIBREPR_WARNING_DISABLE(GCC, "-Wnon-template-friend")

template <class C, auto E>
struct UnionKey {
  constexpr friend auto get_union_accessor_adl(UnionKey);
};

template <class C, auto E, auto V>
struct UnionEntry {
  constexpr friend auto get_union_accessor_adl(UnionKey<C, E>) { return V; }
};

LIBREPR_WARNING_POP

template <typename T>
struct UnionAccessor;

template <typename T, class U>
struct UnionAccessor<T U::*> {
  using class_type = U;
  using type       = T;

  template <T U::*V>
  consteval static bool write() {
    (void)UnionEntry<U, get_tag<T>(), V>{};
    return true;
  }
};

template <auto Accessor>
using union_from_accessor = typename UnionAccessor<decltype(Accessor)>::class_type;
}  // namespace util
template <auto Accessor, auto... Accessors>
concept EnableUnion = util::all_same<util::union_from_accessor<Accessor>, util::union_from_accessor<Accessors>...> &&
                      util::UnionAccessor<decltype(Accessor)>::template write<Accessor>() &&
    (util::UnionAccessor<decltype(Accessors)>::template write<Accessors>() && ...);

template <typename Union, auto E>
constexpr inline auto get_union_accessor = get_union_accessor_adl(util::UnionKey<Union, E>{});

template <typename Union, auto E>
using get_union_type =
    typename util::UnionAccessor<std::remove_const_t<decltype(get_union_accessor<Union, E>)> >::type;

template <typename T>
constexpr inline auto get_tag = util::get_tag<std::remove_const_t<T>>();
}  // namespace librepr
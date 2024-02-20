#pragma once
#include <utility>
#include <cstddef>
#include <memory>
#include <librepr/util/util.h>
#include "list.h"

namespace librepr {
template <typename... Types>
class RefTuple {
private:
  void* data[sizeof...(Types)]{};

public:
  static constexpr auto size = sizeof...(Types);
  using types                = TypeList<Types...>;

  template <detail::ref_convertible_to<Types>... U>
  requires (sizeof...(U) != 1 || !(std::same_as<RefTuple, std::remove_cvref_t<U>> || ...))
  explicit RefTuple(U&&... values) noexcept((noexcept(detail::convert_ref<Types>(std::forward<U>(values))) && ...))
      : data{static_cast<void*>(const_cast<std::remove_cvref_t<Types>*>(
            std::addressof(detail::convert_ref<Types>(std::forward<U>(values)))))...} {}

  RefTuple(RefTuple&&) noexcept                 = default;
  RefTuple(RefTuple const&) noexcept            = default;
  RefTuple& operator=(RefTuple&&) noexcept      = default;
  RefTuple& operator=(RefTuple const&) noexcept = default;
  ~RefTuple()                                   = default;

  template <std::size_t Idx>
    requires(Idx < sizeof...(Types))
  constexpr decltype(auto) get() const noexcept {
    using return_type = types::template get<Idx>;
    return *static_cast<std::remove_reference_t<return_type>*>(data[Idx]);
  }
};

template <>
class RefTuple<> {
public:
  RefTuple() = default;
};

template <typename... Types>
RefTuple(Types&...) -> RefTuple<Types...>;

namespace detail {
struct make_reftuple_impl {
  template <typename... Ts>
  auto operator()(Ts&&... refs) const {
    return RefTuple{std::forward<Ts>(refs)...};
  }
};
}  // namespace detail

constexpr inline auto make_reftuple = detail::make_reftuple_impl{};

template <std::size_t Idx, typename... Ts>
constexpr decltype(auto) get(RefTuple<Ts...> const& tuple) noexcept {
  return tuple.template get<Idx>();
}

}  // namespace librepr

template <std::size_t I, typename... Ts>
struct std::tuple_element<I, librepr::RefTuple<Ts...>> {
  using type = typename librepr::TypeList<Ts...>::template get<I>;
};

template <typename... Ts>
struct std::tuple_size<librepr::RefTuple<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};
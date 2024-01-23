#pragma once
#include <cstddef>
#include <memory>
#include "util.h"
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
    return *static_cast<return_type*>(data[Idx]);
  }
};

template <typename... Types>
RefTuple(Types&...) -> RefTuple<Types...>;

template <std::size_t Idx, typename Tuple>
  requires requires(Tuple const& tuple) { tuple.template get<Idx>(); }
constexpr decltype(auto) get(Tuple const& tuple) noexcept {
  return tuple.template get<Idx>();
}

namespace detail {
struct make_reftuple_impl {
  template <typename... Ts>
  auto operator()(Ts&&... refs) const {
    return RefTuple{std::forward<Ts>(refs)...};
  }
};
}  // namespace detail

constexpr inline auto make_reftuple = detail::make_reftuple_impl{};

}  // namespace librepr
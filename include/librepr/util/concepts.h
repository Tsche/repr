#pragma once
#include <concepts>
#include <type_traits>
#include <string>

namespace librepr::detail {
template <typename T>
concept pair_like = requires(T t) {
  t.first;
  t.second;
  requires std::is_same_v<typename T::first_type, decltype(t.first)>;
  requires std::is_same_v<typename T::second_type, decltype(t.second)>;
};

template <typename T>
concept has_repr_member = requires(T const& obj) {
  { obj.repr() } -> std::same_as<std::string>;
};

template <typename T>
concept is_scoped_enum = !std::is_convertible_v<T, std::underlying_type_t<T>>;

template <typename T, std::size_t N>
concept is_tuple_element = requires(T t) {
  typename std::tuple_element_t<N, std::remove_const_t<T>>;
  { get<N>(t) } -> std::convertible_to<std::tuple_element_t<N, T> const&>;
};
template <typename T>
concept tuple_like = !std::is_reference_v<T> && requires {
  typename std::tuple_size<T>::type;
  requires std::same_as<decltype(std::tuple_size_v<T>), size_t>;
} && []<std::size_t... I>(std::index_sequence<I...>) {
  return (is_tuple_element<T, I> && ...);
}(std::make_index_sequence<std::tuple_size_v<T>>{});

}  // namespace librepr::detail
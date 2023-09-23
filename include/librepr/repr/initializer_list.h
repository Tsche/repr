#pragma once
#include <string>
#include <librepr/detail/visibility.h>

#include <ranges>
#include <sstream>
#include <utility>
#include <initializer_list>

namespace librepr::detail {

template <typename First, typename Second>
std::string to_init_list(std::pair<First, Second> const& obj);

template <std::ranges::range T>
std::string to_init_list(T const&);

}  // namespace librepr::detail

namespace librepr {

template <typename T>
  requires std::constructible_from<std::initializer_list<typename T::value_type>>
EXPORT std::string repr(T const& obj) {
  return librepr::get_name<T>() + detail::to_init_list(obj);
}

template <typename T, std::size_t N>
EXPORT std::string repr(T (&obj)[N]) {
  return std::format("{}", detail::to_init_list(obj));
}

}  // namespace librepr

namespace librepr::detail {

template <typename First, typename Second>
std::string to_init_list(std::pair<First, Second> const& obj) {
  // this exists solely to denoise the output a little, gotta redo this properly at some point
  // std::map etc. are constructible from std::initializer_list<std::pair<key_type, mapped_type>>
  return std::format("{{{}, {}}}", obj.first, obj.second);
}

template <std::ranges::range T>
std::string to_init_list(T const& obj) {
  std::ostringstream list;

  list << '{';
  for (auto const& element : obj) {
    if (&element != &*std::begin(obj)) {
      list << ", ";
    }
    
    if constexpr (requires { to_init_list(element); }) {
      list << to_init_list(element);
    } else {
      list << repr(element);
    } 
  }
  list << '}';
  return list.str();
}

}  // namespace librepr::detail
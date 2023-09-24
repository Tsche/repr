#pragma once
#include <string>
#include <ranges>
#include <sstream>
#include <utility>
#include <initializer_list>
#include <vector>

#include <librepr/detail/visibility.h>
#include <librepr/object_info.h>

namespace librepr::detail {

template <typename First, typename Second>
std::string to_init_list(std::pair<First, Second> const& obj);

template <std::ranges::range T>
std::string to_init_list(T const&);

}  // namespace librepr::detail

namespace librepr {

template <std::ranges::range T>
  requires std::constructible_from<std::initializer_list<typename T::value_type>>
EXPORT auto repr(T const& obj) {
  using repr_t = decltype(librepr::repr(*std::begin(obj)));
  std::vector<repr_t> values{};

  for (auto const& element : obj) {
    values.push_back(librepr::repr(element));
  }

  return ObjectInfo{obj, InitializerList<repr_t>{values}};
}

template <typename T, std::size_t N>
EXPORT ObjectInfo<T const*, Literal> repr(T (&obj)[N]) {
  using repr_t = decltype(librepr::repr(*std::begin(obj)));
  std::vector<repr_t> values{};

  for (auto const& element : obj) {
    values.push_back(librepr::repr(element));
  }

  // convert InitializerList to Literal to prevent readdition of the type name

  return ObjectInfo{static_cast<T const* const>(obj), Literal{InitializerList<repr_t>{values}.to_string(false)}};
};

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
      list << librepr::repr(element).to_string(false, false);
    }
  }
  list << '}';
  return list.str();
}

}  // namespace librepr::detail
#pragma once
#include <string>
#include <librepr/detail/default.h>

#include <librepr/name/ctti.h>
#include <librepr/name/rtti.h>
#include <librepr/demangle.h>

#include <format>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace librepr {


template <typename T>
std::string get_name();

template <typename T>
struct TypeInfo {
  static std::string name() { return librepr::get_name_raw<T>(); }

  static constexpr bool is_fundamental = std::is_fundamental_v<T>;
  static constexpr bool is_templated = false;
};

template <>
struct TypeInfo<std::string> {
  // TODO figure out how to find aliases properly
  static std::string name() { return "std::string"; }

  static constexpr bool is_fundamental = false;
  static constexpr bool is_templated = false;
};

template <template <typename...> typename U, typename... Ts>
struct TypeInfo<U<Ts...>> {
  using parameters = std::tuple<Ts...>;
  static constexpr bool is_fundamental = false;
  static constexpr bool is_templated = true;

private:
  template <std::size_t offset, std::size_t... indices>
  static constexpr auto do_slice(std::index_sequence<indices...>) noexcept
      -> std::tuple<std::tuple_element_t<indices + offset, parameters>...>;

  template <std::size_t start, std::size_t end = sizeof...(Ts)>
  using Slice = decltype(do_slice<start>(std::make_index_sequence<end - start>{}));

  template <std::size_t... Idx>
  consteval static auto get_min_required(std::index_sequence<Idx...> seq) -> std::size_t {
    if constexpr (requires { typename U<std::tuple_element_t<Idx, parameters>...>; }) {
      if constexpr (std::is_same_v<U<Ts...>, U<std::tuple_element_t<Idx, parameters>...>>) {
        if constexpr (sizeof...(Idx) == 0) {
          return 0;
        } else {
          return get_min_required(std::make_index_sequence<seq.size() - 1>());
        }
      }
    }
    return seq.size() + 1;
  }
  constexpr static auto required_amount = get_min_required(std::make_index_sequence<sizeof...(Ts)>());

public:
  using arguments = std::tuple<Ts...>;
  using required = Slice<0, required_amount>;
  using defaulted = Slice<required_amount>;

  static std::string name() {
    using full_type = U<Ts...>;
    auto full       = librepr::get_name_raw<full_type>();

    auto marker = full.find('<');
    if (marker == full.npos) {
      return full;
    }

    auto type      = std::string_view(full.data(), marker);
    auto arguments = format_template_arguments();
    return std::format("{}<{}>", type, arguments);
  }

  static std::string format_template_arguments() {
    constexpr static auto argument_count = USING(REPR_DEFAULT_TEMPLATE_ARGUMENTS) ? sizeof...(Ts) : required_amount;

    return []<std::size_t... I>(std::index_sequence<I...>) {
      const char* sep = "";
      std::ostringstream out;
      (((out << sep << librepr::get_name<std::tuple_element_t<I, parameters>>()), sep = ", "), ...);
      return out.str();
    }(std::make_index_sequence<argument_count>());
  }
};


template <typename T>
char const* get_mangled_name() {
#if defined(_WIN32)
  // This actually returns the mangled name of this function
  // demangle(...) implements a dirty hack to work around that
  return __FUNCDNAME__;
#else
  // On Linux mangled names can only be retrieved using RTTI
  return typeid(T).name();
#endif
}

template <typename T>
std::string get_name() {
  return librepr::TypeInfo<T>::name();
}

}  // namespace librepr
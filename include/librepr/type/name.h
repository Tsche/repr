#pragma once

#include <concepts>
#include <string>
#include <string_view>
#include <sstream>
#include <tuple>
#include <type_traits>

#include <librepr/detail/default.h>
#include <librepr/detail/format.h>
#include <librepr/util/type_list.h>
#include "ctti.h"
#include "rtti.h"

namespace librepr {
template <typename T>
std::string get_name();

namespace detail {

template <typename T>
struct TemplateInfo {
  static std::string name() { return librepr::get_name_raw<T>(); }
  static constexpr bool is_templated = false;
};

template <template <typename...> typename U, typename... Ts>
struct TemplateInfo<U<Ts...>> {
  using type                         = U<Ts...>;
  using arguments                    = TypeList<Ts...>;
  static constexpr bool is_templated = true;

private:
  template <std::size_t... Idx>
  consteval static auto get_min_required(std::index_sequence<Idx...> seq) -> std::size_t {
    if constexpr (requires {
                    typename U<typename arguments::template get<Idx>...>;
                    requires std::same_as<U<Ts...>, U<typename arguments::template get<Idx>...>>;
                  }) {
      if constexpr (sizeof...(Idx) == 0) {
        return 0;
      } else {
        return get_min_required(std::make_index_sequence<seq.size() - 1>());
      }
    }
    return seq.size() + 1;
  }
  constexpr static auto required_amount = get_min_required(std::make_index_sequence<sizeof...(Ts)>());

public:
  using required  = arguments::template slice<0, required_amount>;
  using defaulted = arguments::template slice<required_amount>;

  static std::string name() {
    auto full = librepr::get_name_raw<type>();

    auto marker = full.find('<');
    if (marker == full.npos) {
      return full;
    }

    return REPR_FORMAT("{}<{}>", std::string_view(full.data(), marker), format_template_arguments());
  }

  static std::string format_template_arguments() {
    constexpr static auto argument_count = USING(REPR_DEFAULT_TEMPLATE_ARGUMENTS) ? sizeof...(Ts) : required_amount;

    return []<std::size_t... Idx>(std::index_sequence<Idx...>) {
      const char* sep = "";
      std::ostringstream out;
      
      (((out << sep << TemplateInfo<typename arguments::template get<Idx>>::name()), sep = ", "), ...);
      return out.str();
    }(std::make_index_sequence<argument_count>());
  }
};
}  // namespace detail

template <typename T>
char const* get_mangled_name() {
#if USING(REPR_MSVC)
  if constexpr(msvc_rawname) {
    // Uses vcruntime internals to get a mangled name.
    static auto& ty = typeid(T);
    static volatile auto _ = ty.name();
    return ty.raw_name();
  } else {
    // Returns the mangled name of this function
    // demangle(...) implements a dirty hack to bypass that
    return __FUNCDNAME__;
  }
#else
  return typeid(T).name();
#endif
}

template <typename T>
std::string get_name() {
  return librepr::detail::TemplateInfo<T>::name();
}
}  // namespace librepr
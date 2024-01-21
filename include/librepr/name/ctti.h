#pragma once
#include <string>
#include <librepr/macro/platform.h>
#include <librepr/util/string.h>
#include <librepr/macro/default.h>
namespace librepr::
#if USING(REPR_CTTI)
    inline
#endif
    ctti {
namespace librepr::detail {
template <typename T>
constexpr auto get_ctti() {
#if USING(LIBREPR_COMPILER_MSVC)
  constexpr auto prefix = std::string_view{"get_ctti<"};
  constexpr auto suffix = std::string_view{">(void)"};
#else
  constexpr auto prefix = std::string_view{"T = "};
  constexpr auto suffix = std::string_view{"]"};
#endif

  constexpr auto signature    = std::string_view{LIBREPR_FUNCTION_NAME};
  constexpr std::size_t start = signature.find(prefix) + prefix.size();
  constexpr std::size_t end   = signature.find(suffix, start);

  constexpr auto value = signature.substr(start, end - start);
  return const_string<value.size()>(value);
}

template <typename T>
constexpr inline auto raw_type_name = get_ctti<T>();

}  // namespace librepr::detail

template <typename T>
constexpr inline auto type_name = std::string_view{librepr::detail::raw_type_name<T>};

template <typename T>
std::string get_name_raw() {
  return std::string{type_name<T>};
}
}  // namespace librepr::inline ctti
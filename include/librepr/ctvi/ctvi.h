#pragma once
#include <cstddef>
#include <string_view>

#include <librepr/macro/platform.h>
#include <librepr/macro/warning.h>
#include <librepr/util/string/const_string.h>

namespace librepr::ctvi {

#if USING(LIBREPR_COMPILER_CLANG) && __clang_major__ >= 16
LIBREPR_WARNING_PUSH
// https://github.com/llvm/llvm-project/issues/68489
LIBREPR_WARNING_DISABLE_CLANG("-Wenum-constexpr-conversion")
#endif

namespace detail {
template <auto V>
[[nodiscard]] constexpr auto get_ctvi() noexcept {
#if USING(LIBREPR_COMPILER_MSVC)
  constexpr auto prefix       = std::string_view{"get_ctvi<"};
  constexpr auto suffix       = std::string_view{">(void)"};
#else
  constexpr auto prefix       = std::string_view{"V = "};
  constexpr auto suffix       = std::string_view{"]"};
#endif

  constexpr auto signature    = std::string_view{LIBREPR_FUNCTION_NAME};
  constexpr std::size_t start = signature.find(prefix) + prefix.size();
  constexpr std::size_t end   = signature.find(suffix, start);

  constexpr auto value = signature.substr(start, end - start);
  return const_string<value.size()>(value);
}

template <typename T, auto M>
constexpr auto name_from_subobject() {
  constexpr auto raw = std::string_view{LIBREPR_FUNCTION_NAME};

#if USING(LIBREPR_COMPILER_CLANG)
  constexpr auto start_marker = std::string_view{"."};
  constexpr auto end_marker   = std::string_view{"}]"};
#elif USING(LIBREPR_COMPILER_MSVC)
  constexpr auto start_marker = std::string_view{"->"};
  constexpr auto end_marker   = std::string_view{">(void)"};
#else
  constexpr auto start_marker = std::string_view{"::"};
  constexpr auto end_marker   = std::string_view{")]"};
#endif

  constexpr auto name =
      std::string_view{raw.begin() + raw.rfind(start_marker) + start_marker.length(), raw.end() - end_marker.size()};
  return librepr::const_string<name.length()>(name);
}

#if !USING(LIBREPR_COMPILER_MSVC)
template <auto P>
consteval auto name_from_member_ptr() {
  constexpr auto raw          = std::string_view{__PRETTY_FUNCTION__};
  constexpr auto start_marker = std::string_view{"::"};
  constexpr auto end_marker   = std::string_view{"]"};

  constexpr auto name = std::string_view{raw.begin() + raw.rfind(start_marker) + start_marker.length(), 
                                         raw.end() - end_marker.size()};

  return librepr::const_string<name.length()>(name);
}
#endif
}  // namespace detail

template <auto V>
constexpr inline auto value = detail::get_ctvi<V>();

#if USING(LIBREPR_COMPILER_CLANG) && __clang_major__ >= 16
LIBREPR_WARNING_POP
#endif

}  // namespace librepr::ctvi
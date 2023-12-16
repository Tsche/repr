#pragma once
#include <string_view>
#include <array>
#include <utility>
#include <librepr/detail/platform.h>
#include <librepr/util/strings.h>

namespace librepr::ctvi {

namespace detail {
template <auto V>
[[nodiscard]] constexpr auto get_ctvi() noexcept {
#if defined(LIBREPR_COMPILER_MSVC)
  constexpr auto prefix = std::string_view{"get_ctvi<"};
  constexpr auto suffix = std::string_view{">(void)"};
#else
  constexpr auto prefix = std::string_view{"V = "};  
  constexpr auto suffix = std::string_view{"]"};
#endif

  constexpr auto signature    = std::string_view{LIBREPR_FUNCTION_NAME};
  constexpr std::size_t start = signature.find(prefix) + prefix.size();
  constexpr std::size_t end   = signature.find(suffix, start);

  constexpr auto value = signature.substr(start, end - start);
  return const_string<value.size()>(value);
}

template <typename T, auto M>
constexpr auto name_from_subobject() {
  constexpr auto raw = std::string_view {LIBREPR_FUNCTION_NAME};

#if defined(__clang__)
  constexpr auto start_marker = std::string_view{"."};
  constexpr auto end_marker   = std::string_view{"}]"};
#elif defined(_WIN32)
  constexpr auto start_marker = std::string_view{"->"};
  constexpr auto end_marker   = std::string_view{">(void)"};
#else
  constexpr auto start_marker = std::string_view{"::"};
  constexpr auto end_marker   = std::string_view{")]"};
#endif

  constexpr auto name = std::string_view{std::begin(raw) + raw.rfind(start_marker) + start_marker.length(), std::end(raw) - end_marker.size()};

  return librepr::const_string<name.length()>(name);
}


#if !defined(LIBREPR_COMPILER_MSVC)
template <auto P>
consteval auto name_from_member_ptr() {
    constexpr auto raw = std::string_view{__PRETTY_FUNCTION__};
    constexpr auto start_marker = std::string_view{"::"};
    constexpr auto end_marker = std::string_view{"]"};

    constexpr auto name = std::string_view{
        std::begin(raw) + raw.rfind(start_marker) + start_marker.length(),
        std::end(raw) - end_marker.size()};

    return librepr::const_string<name.length()>(name);
}
#endif
}  // namespace detail

template <auto V>
constexpr inline auto value = detail::get_ctvi<V>();

}  // namespace librepr::ctvi
#include <librepr/detail/default.h>
#include <librepr/type/demangle.h>
#include <librepr/reflection/name.h>

#include <array>
#include <string>
#include <string_view>

#pragma comment(lib, "dbghelp.lib")
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <DbgHelp.h>

static void remove_all(std::string& text, const std::string& from) {
  for (auto at = text.find(from, 0); at != text.npos; at = text.find(from, at)) {
    text.erase(at, from.length());
  }
}

namespace librepr {

namespace detail {

std::string denoise_name(std::string_view name) {
  auto ret = std::string{name.data()};
  remove_all(ret, "struct ");
  remove_all(ret, "class ");
  return ret;
}
}  // namespace detail

std::string demangle(std::string_view mangled) {
  using namespace std::string_view_literals;

  auto buffer = std::array<char, REPR_DEMANGLE_MAX>{};
  auto result = ::UnDecorateSymbolName(mangled.data(), buffer.data(), REPR_DEMANGLE_MAX,
                                       UNDNAME_NAME_ONLY | UNDNAME_NO_MS_KEYWORDS);
  if (not result) {
    // could not demangle
    return mangled.data();
  }
  auto ret = std::string(buffer.data());
  
  static constexpr auto wrapper = "librepr::get_mangled_name<"sv;
  if (ret.starts_with(wrapper)) {
    ret.pop_back();
    ret.erase(ret.begin(), std::next(ret.begin(), wrapper.size()));
  }
  return ret;
}

}  // namespace librepr
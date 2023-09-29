#include <librepr/repr/pointer.h>
#include <format>

namespace librepr {

std::string repr(std::nullptr_t) {
  return "nullptr";
}

std::string repr(char const* obj) {
  return std::format("\"{}\"", obj);
}

std::string repr(void const* obj){
  return std::format("{}", obj);
}
}  // namespace librepr
#include <librepr/repr/pointer.h>

namespace librepr {

ObjectInfo<std::nullptr_t, Literal> repr(std::nullptr_t) {
  return {nullptr, "nullptr"};
}

ObjectInfo<char const*, Literal> repr(char const* obj) {
  return {obj, std::format("\"{}\"", obj)};
}

}  // namespace librepr
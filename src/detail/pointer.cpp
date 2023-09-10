#include <detail/pointer.h>

std::string repr(std::nullptr_t obj) {
  return "nullptr";
}

std::string repr(char const* obj) {
    return std::format("\"{}\"", obj);
}
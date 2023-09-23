#include <librepr/demangle.h>

#include <cxxabi.h>
#include <memory>

namespace librepr {

std::string demangle(const char* mangled) {
  auto const demangled = std::unique_ptr<char, decltype(std::free)*>{
      abi::__cxa_demangle(mangled, nullptr, nullptr, nullptr), std::free};

  return std::string{(demangled) ? demangled.get() : mangled};
};

}
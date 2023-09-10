#pragma once
#include <string>
#include <typeinfo>

#include <cxxabi.h>
#include <memory>

template <typename T>
std::string get_name() {
  //TODO non-rtti variant via __PRETTY_FUNCTION__ or std::source_location
  auto const* name = typeid(T).name();

  auto const demangled = std::unique_ptr<char, decltype(std::free)*>{
    abi::__cxa_demangle(name, nullptr, nullptr, nullptr), 
    std::free
  };

  if (demangled) {
    return std::string{demangled.get()};
  }

  return std::string{name};
};
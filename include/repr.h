#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define REPR_EXPORT __declspec(dllexport)
#else
  #define REPR_EXPORT
#endif

REPR_EXPORT void repr();
REPR_EXPORT void repr_print_vector(const std::vector<std::string> &strings);

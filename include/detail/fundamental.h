#pragma once
#include <string>
#include "visibility.h"


// Boolean type
EXPORT std::string repr(bool obj);

// Integer types
EXPORT std::string repr(signed short obj);
EXPORT std::string repr(unsigned short obj);
EXPORT std::string repr(signed int obj);
EXPORT std::string repr(unsigned int obj);
EXPORT std::string repr(signed long obj);
EXPORT std::string repr(unsigned long obj);
EXPORT std::string repr(signed long long obj);
EXPORT std::string repr(unsigned long long obj);


// Floating-point types
EXPORT std::string repr(float obj);
EXPORT std::string repr(double obj);
EXPORT std::string repr(long double obj);

// Character types
EXPORT std::string repr(char obj);

#pragma once

#include <format>
#include <string>
#include "visibility.h"
#include "typename.h"

EXPORT std::string repr(std::nullptr_t obj);
EXPORT std::string repr(char const* obj);

template <typename T>
requires std::is_pointer<T>::value
EXPORT std::string repr(T obj) {
    return std::format("({}){:p}", get_name<T>(), (void*)obj);
}
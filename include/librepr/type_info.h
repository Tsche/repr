#pragma once
#include <string>

#include <librepr/reflection/reflect.h>
#include <librepr/reflection/name.h>
namespace librepr {


template <typename T>
std::string get_name();

template <typename T>
struct TypeInfo : Reflect<T>, librepr::detail::TemplateInfo<T> {
  using type = T;
  using Reflect<T>::dump;
};


}  // namespace librepr
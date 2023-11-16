#pragma once
#include <string>

#include <librepr/reflection/reflect.h>
#include <librepr/type/name.h>

namespace librepr {
template <typename T>
struct TypeInfo : Reflect<T>, librepr::detail::TemplateInfo<T> {
  using type = T;
  using Reflect<T>::visit;
  using Reflect<T>::layout;
  friend std::ostream& operator<<(std::ostream& ctx, TypeInfo<T> const& obj) { return ctx << obj.name(); }
};

template <typename T>
struct TypeName {
  static std::string to_string() noexcept { return librepr::get_name<T>(); }

  /* implicit */ operator std::string() const noexcept {  // NOLINT
    return to_string();
  }

  friend std::ostream& operator<<(std::ostream& ctx, TypeName<T> const& obj) { return ctx << obj.to_string(); }
};
}  // namespace librepr

namespace librepr {
template <class CharT>
using StrFormatter = REPR_FORMAT_NS::formatter<std::string, CharT>;
}  // namespace librepr

// REPR_FORMAT_NS::format support for librepr::TypeInfo<T>{} and typeinfo<T>
template <class T, class CharT>
struct REPR_FORMAT_RNS::formatter<librepr::TypeInfo<T>, CharT> : librepr::StrFormatter<CharT> {  // NOLINT
  auto format(librepr::TypeInfo<T> const& obj, auto& ctx) const {
    return librepr::StrFormatter<CharT>::format(obj.name(), ctx);
  }
};

// REPR_FORMAT_NS::format support for librepr::TypeName<T>{} and nameof<T>
template <class T, class CharT>
struct REPR_FORMAT_RNS::formatter<librepr::TypeName<T>, CharT> : librepr::StrFormatter<CharT> {  // NOLINT
  auto format(librepr::TypeName<T> const& obj, auto& ctx) const {
    return librepr::StrFormatter<CharT>::format(obj.to_string(), ctx);
  }
};

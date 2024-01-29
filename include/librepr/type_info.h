#pragma once
#include <iosfwd>
#include <string>
#include <string_view>

#include <librepr/util/string/buffer.h>
#include <librepr/macro/format.h>
#include <librepr/name/type.h>
namespace librepr {

template <typename T>
struct TypeName {
  static std::string to_string() noexcept { return librepr::get_name<T>(); }

  /* implicit */ operator std::string() const noexcept {  // NOLINT
    return to_string();
  }

  friend std::ostream& operator<<(std::ostream& ctx, TypeName<T> const& obj) { return ctx << obj.to_string(); }
  friend detail::StringBuffer& operator<<(detail::StringBuffer& ctx, TypeName<T> const& obj) {
    return ctx << obj.to_string();
  }
};
}  // namespace librepr

namespace librepr::detail {
template <class CharT>
using StrFormatter = REPR_FORMAT_NS::formatter<std::string, CharT>;
}  // namespace librepr::detail

// REPR_FORMAT_NS::format support for librepr::TypeName<T>{} and nameof<T>
template <class T, class CharT>
struct REPR_FORMAT_RNS::formatter<librepr::TypeName<T>, CharT> : librepr::detail::StrFormatter<CharT> {  // NOLINT
  auto format(librepr::TypeName<T> const& obj, auto& ctx) const {
    return librepr::detail::StrFormatter<CharT>::format(obj.to_string(), ctx);
  }
};

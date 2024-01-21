#pragma once
#include <string>
#include <librepr/util/string_buffer.h>
#include <librepr/macro/format.h>
#include <librepr/name/type.h>
namespace librepr {

template <typename T>
struct TypeName {
  std::string raw_name;
  std::string_view namespace_{};
  std::string_view stem;

  TypeName() : raw_name(librepr::get_name<T>()) {
    auto name = std::string_view{raw_name};
    if (auto pos = name.rfind("::"); pos != std::string_view::npos) {
      namespace_ = name.substr(0, pos);
      stem       = name.substr(pos + 2);
    } else {
      stem = name;
    }
  }

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

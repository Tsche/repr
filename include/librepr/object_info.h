#pragma once
#include <string>
#include <format>
#include <concepts>
#include <sstream>
#include <vector>

#include <librepr/type_info.h>

namespace librepr {

struct ReprWrapper;

template <typename T>
struct ObjectInfo {
  friend struct librepr::ReprWrapper;
  using type = T;
  T const& value;
  
  static std::string name() {
    return TypeInfo<T>::name();
  }

  static std::string layout() {
    return TypeInfo<T>::layout();
  }

  static std::string dump(T const& obj, bool with_type = true, bool explicit_types = false) {
    return TypeInfo<T>::dump(obj, with_type, explicit_types);
  }

  [[nodiscard]] std::string to_string(bool with_type = true, bool explicit_types = false) const {
    return dump(value, with_type, explicit_types);
  }

  [[nodiscard]] operator std::string() const {
    return to_string(not std::is_fundamental_v<typename TypeInfo<T>::type>);
  
  }

  [[nodiscard]] std::string operator()(bool with_type = true, bool explicit_types = false) const {
    return to_string(with_type, explicit_types);
  }

  friend std::ostream& operator<<(std::ostream& ctx, ObjectInfo<T> const& obj) {
    return ctx << obj.to_string(not std::is_fundamental_v<typename TypeInfo<T>::type>);
  }

  friend bool operator==(ObjectInfo<T> const& obj, std::string_view other){
    return obj.to_string(not std::is_fundamental_v<typename TypeInfo<T>::type>) == other;
  }
};
}  // namespace librepr

// std::format support for librepr::ObjectInfo
template <class T, class CharT>
struct std::formatter<librepr::ObjectInfo<T>, CharT> : std::formatter<std::string, CharT> {  // NOLINT
  auto format(librepr::ObjectInfo<T> const& obj, auto& ctx) const {
    return std::formatter<std::string, CharT>::format(obj.to_string(), ctx);
  }
};

#pragma once

#include <concepts>
#include <cstddef>
#include <stack>
#include <string>
#include <string_view>
#include <type_traits>
#include <librepr/name/type.h>
#include <librepr/reflection/category.h>
#include <librepr/reflection/variant.h>
#include <librepr/reflection/aggregate.h>
#include <librepr/reflection/array.h>
#include <librepr/util/string/buffer.h>
#include <librepr/reflection/reflect.h>

namespace librepr {
namespace detail {
inline void replace_all(std::string& text, std::string_view needle, std::string_view replacement) {
  for (auto at = text.find(needle, 0); at != text.npos; at = text.find(needle, at)) {
    text.replace(at, needle.length(), replacement);
  }
}
}  // namespace detail

struct PythonVisitor {
  std::size_t indent_level  = 0;
  std::size_t indent_amount = 4;
  std::stack<std::string> namespaces{};
  detail::StringBuffer result;

  [[nodiscard]] auto get_indent() const { return std::string(indent_amount * indent_level, ' '); }

  void print_namespaces(std::string_view namespaces_) {
    auto needle = std::string_view{"::"};
    for (auto at = namespaces_.find(needle, 0); at != namespaces_.npos; at = namespaces_.find(needle, at)) {
      result << get_indent() << "class " << namespaces_.substr(0, at) << ":\n";
      namespaces_.remove_prefix(at + 2);
      indent_level++;
    }
    result << get_indent() << "class " << namespaces_ << ":\n";
    indent_level++;
  }

  static auto fix_namespace(std::string_view namespace_) {
    auto ret = std::string{namespace_};
    detail::replace_all(ret, "::", ".");
    return ret;
  }

  template <typename T>
  void print_type() {
    if constexpr (std::is_fundamental_v<T>) {
      if constexpr (std::is_floating_point_v<T>) {
        result << "float";
      } else if constexpr (std::same_as<T, bool>) {
        result << "bool";
      } else if constexpr (std::same_as<T, char>) {
        result << "str";
      } else if constexpr (std::is_integral_v<T>) {
        result << "int";
      } else {
        // todo handle all fundamentals
        result << fix_namespace(get_name<T>());
      }
    } else if constexpr (std::same_as<T, const char*>) {
      result << "str";
    } else if constexpr (requires { typename Reflect<T>::element_type; }) {
      if constexpr (requires { typename T::mapped_type; }) {
        result << "dict[";
        print_type<typename T::key_type>();
        result << ", ";
        print_type<typename T::mapped_type>();
        result << "]";
      } else {
        result << "list[";
        print_type<typename Reflect<T>::element_type>();
        result << "]";
      }
    } else if constexpr (is_variant<T>) {
      Reflect<T>::alternatives::enumerate([this]<typename Member, std::size_t Index>() {
        if constexpr (Index != 0) {
          result << " | ";
        }
        print_type<Member>();
      });
    } else {
      result << fix_namespace(get_name<T>());
    }
  }

  template <typename T>
    requires category::has_members<T>
  auto operator()(T info) {
    auto full_name = info.type_name();
    auto name      = std::string_view{full_name};
    std::string_view namespace_{};
    bool out_of_line = false;
    auto last_indent = indent_level;

    if (!namespaces.empty()) {
      if (name.starts_with(namespaces.top())) {
        name.remove_prefix(namespaces.top().size() + 2);
      } else {
        // namespace mismatch -> defined out of line
        result.set_cursor(0);
        indent_level = 0;
        out_of_line  = true;
        if (auto pos = name.rfind("::"); pos != std::string_view::npos) {
          namespace_ = name.substr(0, pos);
          name       = name.substr(pos + 2);
          print_namespaces(namespace_);
        }
      }
    }

    result << get_indent() << "@dataclass\n";
    result << get_indent() << "class " << name << ":\n";

    namespaces.push(full_name);
    indent_level++;
    if constexpr (T::members::size == 0) {
      result << get_indent() << "...\n";
    } else {
      info.visit(*this);
    }
    indent_level--;
    namespaces.pop();

    if (out_of_line) {
      indent_level = last_indent;  // roll back indent level
      result.set_cursor();         // roll back cursor
    }

    result << get_indent();
    if constexpr (category::has_name<T>) {
      result << info.name() << ": ";
      if (!namespace_.empty()) {
        result << fix_namespace(namespace_) << '.';
      }
      result << name << '\n';
    }
  }

  template <typename T>
  auto operator()(T info) {
    result << get_indent() << info.name() << ": ";
    print_type<typename T::type>();
    result << '\n';
  }
};
}  // namespace librepr
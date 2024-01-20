#pragma once

#include <type_traits>
#include <cstddef>
#include <string>
#include <string_view>
#include <librepr/reflection/aggregate.h>
#include <librepr/reflection/category.h>
#include <librepr/util/string_buffer.h>
#include <librepr/reflection/reflect.h>

namespace librepr {
struct CppVisitor {
  std::size_t indent_level  = 0;
  std::size_t indent_amount = 4;
  std::size_t unnamed_count = 0;
  detail::StringBuffer result;

private:
  template <typename T>
    requires category::has_name<T>
  auto name(T const& info) {
    return std::string{info.name()};
  }

  template <typename T>
  auto name(T const&) {
    ++unnamed_count;
    return std::format("UNNAMED_{}", unnamed_count);
  }

  template <typename T>
  void print_name(T const& info) {
    result << ' ' << name(info);
  }

  [[nodiscard]] auto get_indent() const { return std::string(indent_amount * indent_level, ' '); }

  template <typename T>
    requires category::has_parent<T>
  bool is_out_of_line(T const& info) {
    auto parent_name = T::parent::type_name();
    return !info.type_name().starts_with(parent_name);
  }

  template <typename F>
  void defer(F&& fnc) {
    auto last_indent = indent_level;
    result.set_cursor(0);
    indent_level = 0;
    fnc();
    indent_level = last_indent;
    result.set_cursor();
  }

public:
  template <typename T>
    requires category::has_members<T>
  auto operator()(T info) {
    auto full_name = info.type_name();

    auto name = std::string_view{full_name.raw_name};
    std::string_view namespace_{};
    bool out_of_line = false;
    auto last_indent = indent_level;

    if constexpr (category::has_parent<T>) {
      auto parent_name = T::parent::type_name();
      if (name.starts_with(parent_name.raw_name)) {
        name.remove_prefix(parent_name.raw_name.size() + 2);
      } else {
        // namespace mismatch -> defined out of line
        result.set_cursor(0);
        indent_level = 0;
        out_of_line  = true;
        if (auto pos = name.rfind("::"); pos != std::string_view::npos) {
          namespace_ = name.substr(0, pos);
          name       = name.substr(pos + 2);
          result << get_indent() << "namespace " << namespace_ << "{ \n";
        }
      }
    }

    result << get_indent() << "struct " << name << " {\n";

    indent_level++;
    info.visit(*this);
    indent_level--;

    result << get_indent() << '}';
    if (out_of_line) {
      result << ";\n";

      if (!namespace_.empty()) {
        result << "}\n";
      }

      indent_level = last_indent;           // roll back indent level
      result.set_cursor();                  // roll back cursor
      result << get_indent() << full_name;  // output type name again
    }

    if constexpr (category::has_name<T>) {
      result << ' ' << info.name();
    }
    result << ";\n";
  }

  template <typename T>
    requires librepr::category::has_enumerator_names<T>
  auto operator()(T info) {
    result << get_indent() << "enum ";
    if constexpr (std::is_scoped_enum_v<typename T::type>) {
      result << "class ";
    }
    result << info.type_name() << " {\n";
    ++indent_level;
    for (auto const& enumerator : info.enumerator_names) {
      result << get_indent() << enumerator << ",\n";
    }
    --indent_level;
    result << get_indent() << "}";
    print_name(info);
    result << ";\n";
  }

  template <typename T>
    requires category::has_extent<T>
  auto operator()(T info) {
    result << get_indent() << get_name<typename T::element_type>();
    print_name(info);
    result << '[' << std::to_string(info.extent) << "];\n";
  }

  template <typename T>
  auto operator()(T info) {
    result << get_indent() << info.type_name();
    print_name(info);
    result << ";\n";
  }
};
}  // namespace librepr
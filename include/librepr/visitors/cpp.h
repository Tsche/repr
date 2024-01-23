#pragma once

#include <cstddef>
#include <format>
#include <string>
#include <string_view>
#include <type_traits>

#include <librepr/name/type.h>
#include <librepr/reflection/aggregate.h>
#include <librepr/reflection/category.h>
#include <librepr/util/concepts.h>
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
    if constexpr (std::is_const_v<typename T::type>){
      result << " const";
    }
    result << ' ' << name(info);
  }

  [[nodiscard]] auto get_indent() const { return std::string(indent_amount * indent_level, ' '); }

  void print_struct(category::has_members auto info, std::string_view name) {
    result << get_indent() << "struct " << name << " {\n";

    indent_level++;
    info.visit(*this);
    indent_level--;

    result << get_indent() << '}';
  }

public:
  template <category::has_members T>
  auto operator()(T info) {
    auto full_name = info.type_name();
    auto name = std::string_view{full_name};

    if constexpr (category::has_parent<T>) {
      auto parent_name = T::parent::type_name();
      if (name.starts_with(parent_name)) {
        name.remove_prefix(parent_name.size() + 2);
        print_struct(info, name);
      } else {
        // namespace mismatch -> defined out of line
        result.set_cursor(0);
        auto last_indent = indent_level;
        indent_level = 0;

        std::string_view namespace_{};
        if (auto pos = name.rfind("::"); pos != std::string_view::npos) {
          namespace_ = name.substr(0, pos);
          name       = name.substr(pos + 2);
        }
        if (!namespace_.empty()) {
          result << get_indent() << "namespace " << namespace_ << " { \n";
        }

        print_struct(info, name);
        result << ";\n";

        if (!namespace_.empty()) {
          result << "}\n";
        }
        indent_level = last_indent;
        result.set_cursor();
        result << get_indent() << full_name;  // output type name
      }
    } else {
      print_struct(info, name);
    }

    if (indent_level != 0){
      print_name(info);
    }
    result << ";\n";
  }

  template <typename T>
    requires librepr::category::has_enumerator_names<T>
  auto operator()(T info) {
    result << get_indent() << "enum ";
    constexpr auto is_scoped = detail::is_scoped_enum<typename T::type>;
    if constexpr (is_scoped) {
      result << "class ";
    }
    
    auto raw_name = info.type_name();
    auto name = std::string_view{raw_name};
    if constexpr (category::has_parent<T>){
      auto parent_name = T::parent::type_name();
      if (name.starts_with(parent_name)) {
        name.remove_prefix(parent_name.size() + 2);
      }
    }
    
    result << name << " {\n";
    ++indent_level;
    for (auto const& enumerator : info.enumerator_names) {
      auto name = (is_scoped) ? enumerator : enumerator.substr(enumerator.rfind("::") + 2);
      result << get_indent() << name << ",\n";
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
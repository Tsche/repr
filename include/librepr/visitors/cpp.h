#include <stack>
#include <type_traits>
#include <cassert>
#include <format>
#include <librepr/options.h>
#include <librepr/name/type.h>

struct CppVisitor {
private:
  std::size_t level                  = 0;
  std::stack<std::string_view> names = {};
  librepr::Options options           = {};

  void print_indent() { result.append(std::string(options.indent * level, ' ')); }

public:
  std::string result{};
  explicit CppVisitor(librepr::Options const& options_) : options(options_) {}

  void nesting(bool increase) {
    if (increase) {
      ++level;

      result.append("{\n");

    } else {
      --level;
      print_indent();
      result.append("}");

      if (!names.empty()) {
        result.append(" ");
        result.append(names.top());
        names.pop();
      }
      result.append(";\n");
    }
  }

  void member_name(std::string_view name) { names.push(name); }

  template <typename T>
  void print_function(std::size_t array_extent = 0) {
    print_indent();
    auto type_name = librepr::get_name<T>();
    if (!names.empty()) {
      // named member - look for insertion point
      auto insertion_position = type_name.find("(*)");
      assert(insertion_position != std::string::npos);

      result.append(std::string_view{type_name}.substr(0, insertion_position + 2));
      result.append(names.top());

      if (array_extent != 0) {
        result.append(std::format("[{}]", array_extent));
      }
      result.append(std::string_view{type_name}.substr(insertion_position + 2));

      names.pop();
    } else {
      result.append(librepr::get_name<T>());
    }
    result.append(";\n");
  }

  template <typename T>
    requires(std::is_function_v<std::remove_pointer_t<T>>)
  void type() {
    print_function<T>();
  }

  template <typename T>
    requires(std::is_array_v<T>)
  void type() {
    using member_type = std::remove_all_extents_t<T>;
    auto extent       = std::extent_v<T>;
    auto type_name    = librepr::get_name<member_type>();

    if constexpr (std::is_function_v<std::remove_pointer_t<member_type>>) {
      // member_type may be pointer to function - remove pointer and see if it's a function
      print_function<member_type>(extent);
    } else {
      print_indent();
      auto name = names.empty() ? std::string_view{} : names.top();
      result.append(std::format("{} {}[{}]", type_name, name, extent));

      if (!names.empty()) {
        names.pop();
      }
      result.append(";\n");
    }
  }

  template <typename T>
    requires(std::is_aggregate_v<T> && !std::is_array_v<T>)
  void type() {
    print_indent();
    result.append("struct ");
    result.append(librepr::get_name<T>());
    result.append(" ");
  }

  template <typename T>
  void type() {
    print_indent();
    result.append(librepr::get_name<T>());

    if (!names.empty()) {
      result.append(" ");
      result.append(names.top());
      names.pop();
    }
    result.append(";\n");
  }
};
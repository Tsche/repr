#pragma once

#include <string>
#include <format>
#include <type_traits>

#include <librepr/visitors/visitor.h>
#include <librepr/util/concepts.h>

#include <librepr/repr/all.h>
#include <librepr/type/name.h>
#include <librepr/reflection/reflect.h>
#include <librepr/literal.h>
#include <librepr/options.h>

namespace librepr {
class ReprVisitor {
private:
  bool separate     = false;
  std::size_t level = 0;
  Options options   = {};

  void print_indent() {
    if (options.indent != 0) {
      result.append("\n");
      result.append(std::string(options.indent * level, ' '));
    }
  }

  void print_separator() {
    if (separate) {
      result.append(", ");
    } else {
      separate = true;
    }
    print_indent();
  }

public:
  explicit ReprVisitor(Options const& options_) : options(options_) {}
  std::string result{};

  template <typename T>
  void operator()() {
    emit_type<T>();
  }

  template <typename T>
  void operator()(T const& obj) {
    emit_value(obj);
  }

  template <typename T>
  void emit_value(T const& obj) {
    emit_type<T>();
    result += librepr::repr(obj);
  }

  template <typename T>
    requires detail::has_repr_member<T>
  void emit_value(T const& obj) {
    if constexpr (!librepr::is_literal_v<T>) {
      emit_type<T>();
      increase_nesting();
    }

    result += obj.repr();

    if constexpr (!librepr::is_literal_v<T>) {
      decrease_nesting();
    }
  }

  // special case string literals
  void emit_value(char const* obj) {
    // TODO template this for wide string literals
    //  don't print a type, only print the separator if needed
    print_separator();
    result.append(std::format("\"{}\"", obj));
  }

  template <typename T>
    requires std::is_pointer_v<T>
  void emit_value(T const& obj) {
    // don't print a type, only print the separator if needed
    print_separator();
    using underlying_type = std::remove_pointer_t<T>;

    if constexpr (!std::is_same_v<underlying_type, void> && !std::is_pointer_v<underlying_type>) {
      // try to reflect whatever the pointer is pointing to
      // disabled for void* and pointer-to-pointer T

      if (obj) {  // don't attempt to dereference nullptr
        result += "new " + librepr::get_name<underlying_type>();
        increase_nesting();
        librepr::Reflect<underlying_type>::visit(*this, *obj);
        decrease_nesting();
        return;
      }
    }
    result += '(' + librepr::get_name<T>() + ')' + librepr::repr(static_cast<const void*>(obj));
  }

  template <typename T>
  void emit_type() {
    print_separator();
    if constexpr (!librepr::is_literal_v<T>) {
      if (options.should_print_type(level)) {
        result.append(librepr::get_name<T>());
      }
    }
  }

  void increase_nesting() {
    separate = false;
    result += '{';
    ++level;
  }

  void decrease_nesting() {
    --level;
    print_indent();
    result += '}';
  }
};

static_assert(Visitor::Hierarchical<ReprVisitor>, "Formatter isn't a valid hierarchical visitor.");

}  // namespace librepr
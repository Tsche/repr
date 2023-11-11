#pragma once

#include <string>
#include <format>

#include <librepr/reflection/visitor.h>
#include <librepr/detail/concepts.h>

#include <librepr/repr.h>
#include <librepr/reflection/name.h>
#include <librepr/literal.h>

namespace librepr {
class ReprVisitor {
private:
  bool separate      = false;
  std::size_t level  = 0;
  std::size_t indent = 0;

  void print_indent() {
    if (indent != 0) {
      result.append("\n");
      result.append(std::string(indent * level, ' '));
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
  explicit ReprVisitor(std::size_t indent_ = 0) : indent(indent_) {}
  std::string result{};

  template <typename T>
  void operator()() {
    print_separator();

    if constexpr(detail::is_scoped_enum<T>) {
      result.append(librepr::get_name<T>());
      result.append("::");
    }

    if constexpr (!librepr::is_literal_v<T> && !std::is_enum_v<T>) {
      result.append(librepr::get_name<T>());
    }
  }

  void operator()(char const* obj) {
    result.append(std::format("\"{}\"", obj));
  }

  template <typename T>
  void operator()(T const& obj) {
    operator()<T>();
    if constexpr (librepr::detail::has_repr_member<T>) {
      result.append(obj.repr());
    } else {
      // delegate to librepr::repr() here
      result.append(librepr::repr(obj));
    }
  }

  void increase_nesting() {
    separate = false;
    result.append("{");
    ++level;
  }

  void decrease_nesting() {
    --level;
    print_indent();
    result.append("}");
  }
};

static_assert(ObjectVisitor<ReprVisitor>, "Formatter isn't a valid visitor.");

}  // namespace librepr
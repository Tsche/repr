#pragma once

#include <string>
#include <format>
#include <type_traits>

#include <librepr/reflection/visitor.h>
#include <librepr/detail/concepts.h>

#include <librepr/repr.h>
#include <librepr/reflection/name.h>
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
    print_separator();
    if constexpr (!librepr::is_literal_v<T>) {
      if (options.should_print_type(level)) {
        result.append(librepr::get_name<T>());
      }
    }
  }

  template <typename T>
    requires std::is_enum_v<T>
  void operator()() {
    print_separator();
    if constexpr (detail::is_scoped_enum<T>) {
      result.append(librepr::get_name<T>());
      result.append("::");
    }
  }

  void operator()(char const* obj) {
    print_separator();
    result.append(std::format("\"{}\"", obj));
  }

  template <typename T>
  void operator()(T const& obj) {
    if constexpr (std::is_pointer_v<T>) {
      print_separator();
      using underlying_type = std::remove_pointer_t<T>;

      if constexpr (!std::is_same_v<underlying_type, void> && !std::is_pointer_v<underlying_type>) {
        // try to reflect whatever the pointer is pointing to
        // disabled for void* and pointer-to-pointer T

        if (obj) {
          // don't attempt to dereference nullptr
          result.append(std::format("new {}", librepr::get_name<underlying_type>()));
          increase_nesting();
          librepr::Reflect<underlying_type>::visit(*this, *obj);
          decrease_nesting();
          return;
        }
      }
      result.append(std::format("({}){}", librepr::get_name<T>(), librepr::repr(static_cast<void const*>(obj))));

    } else {
      operator()<T>();
      if constexpr (librepr::detail::has_repr_member<T>) {
        result.append(obj.repr());
      } else {
        // delegate to librepr::repr() here
        result.append(librepr::repr(obj));
      }
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
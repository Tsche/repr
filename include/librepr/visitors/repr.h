#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

#include <librepr/reflection/category.h>
#include <librepr/reflection/reflect.h>

#include <librepr/util/string_buffer.h>
#include <librepr/util/concepts.h>
#include <librepr/macro/format.h>
#include <librepr/repr/all.h>
#include <librepr/name/type.h>
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
      result << "\n";
      result << std::string(options.indent * level, ' ');
    }
  }

  void print_separator() {
    if (separate) {
      result << ", ";
    } else {
      separate = true;
    }
    print_indent();
  }

public:
  explicit ReprVisitor(Options const& options_) : options(options_) {}
  detail::StringBuffer result{};

  
  void nesting(bool increase) {
    if (increase) {
      separate = false;
      result << '{';
      ++level;
    }
    else {
      --level;
      print_indent();
      result << '}';
      separate = true;
    }
  }
  
  void member_name(std::string_view name){
    print_separator();
    result << '.'<< name << '=';
    separate = false;
  }

  template <typename T>
  void type() {
    print_separator();
    if constexpr (!librepr::is_literal_v<T>) {
      if (options.should_print_type(level)) {
        result << librepr::get_name<T>();
      }
    }
  }

  template <typename T>
  void value(T const& obj) {
    type<T>();
    result << librepr::repr(obj);
  }

  template <typename T>
    requires detail::has_repr_member<T>
  void value(T const& obj) {
    if constexpr (!librepr::is_literal_v<T>) {
      type<T>();
      nesting(true);
    }

    result << obj.repr();

    if constexpr (!librepr::is_literal_v<T>) {
      nesting(false);
    }
  }

  // special case string literals
  void value(char const* obj) {
    // TODO template this for wide string literals
    //  don't print a type, only print the separator if needed
    print_separator();
    result << REPR_FORMAT("\"{}\"", obj);
  }

  template <typename T>
    requires std::is_pointer_v<T>
  void value(T const& obj) {
    // don't print a type, only print the separator if needed
    print_separator();
    using underlying_type = std::remove_pointer_t<T>;

    if constexpr (!std::is_same_v<underlying_type, void> && !std::is_pointer_v<underlying_type>) {
      // try to reflect whatever the pointer is pointing to
      // disabled for void* and pointer-to-pointer T

      if (obj) {  // don't attempt to dereference nullptr
        result << "new " << librepr::get_name<underlying_type>();
        nesting(true);
        librepr::Reflect<underlying_type>::visit(*this, *obj);
        nesting(false);
        return;
      }
    }
    result << '(' << librepr::get_name<T>() << ')' << librepr::repr(static_cast<const void*>(obj));
  }
  

  template <typename T>
  requires category::has_value<T>
  void operator()(T info){
    if constexpr (category::has_name<T>){
      member_name(info.name());
    }
    value(info.value());
  }

  template <typename T>
  requires category::has_value<T> && category::can_descend<T>
  void operator()(T info){
    if constexpr (category::has_name<T>){
      member_name(info.name());
    }

    type<typename T::type>();
    nesting(true);
    info.visit(*this);
    nesting(false);
  }
};

//static_assert(Visitor::Hierarchical<ReprVisitor>, "Formatter isn't a valid hierarchical visitor.");

}  // namespace librepr
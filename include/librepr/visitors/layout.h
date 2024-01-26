#pragma once
#include <librepr/util/string_buffer.h>
#include <librepr/reflection/category.h>
#include <librepr/name/type.h>
#include <type_traits>

namespace librepr {
struct LayoutVisitor {
  detail::StringBuffer result;
  bool separate = false;

  void print_separator() {
    if (separate) {
      result << ", ";
    } else {
      separate = true;
    }
  }

  template <typename T>
  void print_name() {
    if constexpr (std::is_pointer_v<T> && std::same_as<std::remove_const_t<std::remove_pointer_t<T>>, char>){
      result << "str";
    }
    else{
      result << get_name<T>();
    }
  }

  template <category::can_descend T>
  void descend(T info){
    separate = false;
    info.visit(*this);
  }

  template <typename T>
  void operator()(T info) {
    using type = typename T::type;
    print_separator();

    if constexpr (requires { typename type::value_type; }) {
      // iterable container

      result << '[';
      if constexpr (requires { typename type::key_type; typename type::mapped_type; }){
        // associative container
        print_name<typename type::key_type>();
        result << " -> ";
        print_name<typename type::mapped_type>();
      }
      else {
        print_name<typename type::value_type>();
      }
      
      result << ']';
    } else {
      print_name<type>();
    }
  }

  template <category::has_members T>
  void operator()(T info) {
    print_separator();
    result << '{';
    descend(info);
    result << '}';
  }

  template <category::has_alternatives T>
  void operator()(T info) {
    print_separator();
    result << '<';
    descend(info);
    result << '>';
  }

  template <category::has_enumerator_names T>
  void operator()(T info) {
    result << '(';
    descend(info);
    result << ')';
  }
};
}  // namespace librepr
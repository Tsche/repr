#pragma once
#include <librepr/util/string_buffer.h>
#include <librepr/reflection/category.h>
#include <librepr/name/type.h>
#include <librepr/visit.h>
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
    if constexpr (std::is_pointer_v<T> && std::same_as<std::remove_const_t<std::remove_pointer_t<T>>, char>) {
      result << "str";
    } else {
      result << get_name<T>();
    }
  }

  template <category::can_descend T>
  void descend(T info) {
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
      if constexpr (requires {
                      typename type::key_type;
                      typename type::mapped_type;
                    }) {
        // associative container
        separate = false;
        librepr::visit<typename type::key_type>(*this);
        result << " -> ";
        separate = false;
        librepr::visit<typename type::mapped_type>(*this);
      } else {
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
    T::alternatives::for_each([this, first=true]<typename U>() mutable {
      if (!first) {
        result << " | ";
      }
      first = false;
      separate = false;
      librepr::visit<U>(*this);
    });
  }

  template <category::has_enumerator_names T>
  void operator()(T info) {
    print_separator();
    for (auto const& name : info.enumerator_names) {
      if (&*info.enumerator_names.begin() != &name) {
        result << " | ";
      }
      result << name;
    }
  }

  template <category::has_extent T>
  requires requires { typename T::element_type; }
  void operator()(T info) {
    print_name<typename T::element_type>();
    result << '[' << std::to_string(info.extent) << ']';
  }
};
}  // namespace librepr
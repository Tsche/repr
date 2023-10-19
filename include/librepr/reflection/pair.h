#pragma once
#include <format>
#include <string>
#include <type_traits>

#include <librepr/detail/concepts.h>
#include <librepr/reflection/name.h>

namespace librepr {
template <typename T>
struct Reflect;

template <detail::pair_like T>
struct Reflect<T> {
  using type        = T;
  using first_type  = std::remove_cv_t<std::remove_reference_t<typename T::first_type>>;
  using second_type = std::remove_cv_t<std::remove_reference_t<typename T::second_type>>;

  static std::string dump(T const& obj, bool with_type = true, bool explicit_types = false) {
    return std::format("{}{{{}, {}}}", with_type ? librepr::get_name<T>() : "",
                       Reflect<first_type>::dump(obj.first, explicit_types, explicit_types),
                       Reflect<second_type>::dump(obj.second, explicit_types, explicit_types));
  }

  static std::string layout() {
    return std::format("{{{}, {}}}", Reflect<first_type>::layout(), Reflect<second_type>::layout());
  }
};

}  // namespace librepr
#pragma once
#include <string>

#include <librepr/customization/members.h>
#include <librepr/customization/reflection.h>


namespace librepr {
template <typename T>
struct Reflect;

//TODO this shouldn't be a specialization of Reflect
//! we need to look up in order 
//! settings -> adl settings -> member function
/*template <typename T>
  requires has_custom_members<T>
struct Reflect<T> {
  using type        = T;
  using members = typename Settings<T>::members;

  static_assert(members::template is_valid_for<T>, "Members visit the wrong class type.");

  template <typename V>
  static void visit(V&& visitor, T const& obj) {
    Visit::type<T>(visitor);
    ScopeGuard guard{visitor};

    members::for_each([&visitor, &obj]<Member member> {
      constexpr static auto name = detail::custom_member_name<member>();
      if constexpr (!name.empty()){
        Visit::member_name(visitor, std::string_view{name});
      }
      Reflect<typename decltype(member)::type>::visit(visitor, member(obj));
    });
  }
};*/
}  // namespace librepr
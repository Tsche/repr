#pragma once
#include <type_traits>
#include <string_view>
#include <concepts>
#include <utility>

#include <librepr/util/collections/list.h>
#include <librepr/ctvi/ctvi.h>
#include <librepr/util/string/const_string.h>

namespace librepr {
  
template <typename T>
struct Accessor;

template <typename C, typename T>
struct Accessor<T C::*>
{
private:
    constexpr static auto get_type(){
        if constexpr (std::is_function_v<T>){
            return std::type_identity<std::invoke_result_t<T C::*, C>>{};
        }
        else {
            return std::type_identity<T>{};
        }
    }
public:
    T C::*value;

    using class_type = C;
    using type = typename decltype(get_type())::type;
       
    template <typename Obj>
    requires (std::same_as<Obj, class_type>)
    constexpr decltype(auto) operator()(Obj&& obj) noexcept{
        if constexpr (std::is_function_v<T>){
            return (std::forward<Obj>(obj).*value)();
        }
        else {
            return std::forward<Obj>(obj).*value;
        }
    }
};

template <typename T, typename C>
struct Accessor<T(*)(C const&)>{
    T(*value)(C const&);

    using class_type = C;
    using type = T;

    template <typename Obj>
    requires (std::same_as<Obj, class_type>)
    constexpr decltype(auto) operator()(Obj&& obj) noexcept{
        return value(std::forward<Obj>(obj));
    }
};

template <typename C, typename T>
Accessor(T C::*) -> Accessor<T C::*>;

template <typename C, typename T>
Accessor(T (*)(C const&)) -> Accessor<T (*)(C const&)>;

template <typename T, std::size_t N>
struct Member
{
    using accessor = Accessor<T>;
    using type = typename accessor::type;
    using class_type = typename accessor::class_type;

    Accessor<T> value;
    const_string<N> name{};

    constexpr explicit Member(T accessor) requires (N == 0) : value(accessor) {}
    constexpr Member(T accessor, auto name_) : value(accessor), name(name_) {}

    template <typename Obj>
    requires (std::same_as<std::decay_t<Obj>, class_type>)
    constexpr decltype(auto) operator()(Obj&& obj) noexcept {
        return value(std::forward<Obj>(obj));
    }
};


template <typename T, std::size_t N>
Member(T, char const (&)[N]) -> Member<T, N - 1>;

template <typename T, std::size_t N>
Member(T, const_string<N>) -> Member<T, N>;

template <typename T>
Member(T) -> Member<T, 0>;

template <Member... Ts>
concept is_homogeneous_members = requires { typename std::common_type_t<typename decltype(Ts)::class_type...>; };

template <Member... Ts>
struct MemberList : ValueListBase<MemberList, Ts...> {
    static_assert(is_homogeneous_members<Ts...>, "No common class types in declared members detected.");

    template <typename C>
    constexpr static bool is_valid_for = std::same_as<std::common_type_t<typename decltype(Ts)::class_type...>, C>;
};
}
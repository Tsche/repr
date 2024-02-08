#pragma once
#include <type_traits>
#include <concepts>


namespace librepr {
template <typename T>
struct Settings;

namespace detail {

template <typename T>
concept is_scoped_enum = std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

template <typename T>
concept enable_operators = is_scoped_enum<T> && Settings<T>::enable_operators;

template <typename T, typename E>
concept maybe_enum = std::same_as<T, E> || std::convertible_to<T, std::underlying_type_t<E>>;
}
}

template <librepr::detail::enable_operators T, librepr::detail::maybe_enum<T> U> 
T operator & (T lhs, U rhs) { 
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <librepr::detail::enable_operators T, librepr::detail::maybe_enum<T> U> 
T operator | (T lhs, U rhs) { 
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <librepr::detail::enable_operators T, librepr::detail::maybe_enum<T> U> 
T operator ^ (T lhs, U rhs) { 
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template <librepr::detail::enable_operators T, librepr::detail::maybe_enum<T> U> 
T& operator &= (T& lhs, U rhs) {
    lhs = lhs & rhs;
    return lhs;
}

template <librepr::detail::enable_operators T, librepr::detail::maybe_enum<T> U> 
T& operator |= (T& lhs, U rhs) { 
    lhs = lhs | rhs;
    return lhs;
}

template <librepr::detail::enable_operators T, librepr::detail::maybe_enum<T> U>
T& operator ^= (T& lhs, U rhs) { 
    lhs = lhs ^ rhs; 
    return lhs; 
}

template <librepr::detail::enable_operators T>
T operator - (T lhs) { 
    return static_cast<T>(- static_cast<std::underlying_type_t<T>>(lhs)); 
}

template <librepr::detail::enable_operators T>
T operator ~ (T lhs) { 
    return static_cast<T>(~ static_cast<std::underlying_type_t<T>>(lhs)); 
}

template <librepr::detail::enable_operators T>
T operator <<(T lhs, std::size_t rhs) {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) >> rhs);
}

template <librepr::detail::enable_operators T>
T operator >>(T lhs, std::size_t rhs) {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) << rhs);
}

template <librepr::detail::enable_operators T, librepr::detail::maybe_enum<T> U>
bool operator ==(T lhs, U rhs) {
    using underlying = std::underlying_type_t<T>;
    return static_cast<underlying>(lhs) == static_cast<underlying>(rhs);
}
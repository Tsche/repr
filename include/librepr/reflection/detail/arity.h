#pragma once
#include <type_traits>

namespace librepr::detail {
struct Universal {
    template <typename T>
    [[implicit]] constexpr operator T(); // NOLINT
};

template <typename T>
    requires std::is_aggregate_v<T>
struct ArityImpl {
    template <typename... Fillers>
    static consteval auto array_length(auto... parameters) {
        if constexpr (requires { T{parameters..., {Fillers{}..., Universal{}}}; }) {
            return array_length<Fillers..., Universal>(parameters...);
        } else {
            return sizeof...(Fillers);
        }
    }

    static consteval auto arity_simple(auto... parameters) {
        if constexpr (requires { T{parameters..., Universal{}}; }) {
            return arity_simple(parameters..., Universal{});
        } else {
            return sizeof...(parameters);
        }
    }

    template <typename... Trails>
    static consteval auto arity_simple_ag(auto... parameters) {
        if constexpr (requires { T{parameters..., {Universal{}, Universal{}}, Trails{}..., Universal{}}; }) {
            return arity_simple_ag<Trails..., Universal>(parameters...);
        } else {
            return sizeof...(parameters) + sizeof...(Trails) + 1;
        }
    }

    static consteval auto arity(std::size_t minus = 0, auto... parameters) {
        if constexpr (requires { T{parameters..., {Universal{}, Universal{}}}; }) {
            constexpr bool isArray = arity_simple_ag(parameters...) != arity_simple(parameters...);
            if constexpr (isArray) {
                minus += array_length(parameters...) - 1;
            }

            return arity(minus, parameters..., Universal{});
        }else if constexpr (requires { T{parameters..., Universal{}}; }) {
            return arity(minus, parameters..., Universal{});
        } else {
            return sizeof...(parameters) - minus;
        }
    }
};

template <typename T>
    requires std::is_aggregate_v<T>
inline constexpr std::size_t arity = ArityImpl<T>::arity();

}  // namespace librepr

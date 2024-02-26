@include("../settings.em")

#include <bit>
#include "eval.h"

template <typename T, auto Value>
[[nodiscard]] consteval T to_enum() noexcept {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(Value));
}

enum Unscoped { Foo=7 };
enum class Scoped{ Bar=7 };

@{
enum_constexpr_conversion = ".*-Wenum-constexpr-conversion"
}@

@test("unscoped enum without fixed underlying type"){
    $eval(to_enum<Unscoped, 42>());
    @return_code(gcc, 0)
    @return_code(msvc, 0)

    @error(Clang(version='>=16'), regex=enum_constexpr_conversion)
}

@test("scoped enum without fixed underlying type"){
    $eval(to_enum<Unscoped, 42>());
    @return_code(gcc, 0)
    @return_code(msvc, 0)

    @error(Clang(version='>=16'), regex=enum_constexpr_conversion)
}

int main() {}
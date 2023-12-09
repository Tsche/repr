#pragma once
#include <librepr/ctvi/ctvi.h>

namespace librepr {

template <auto V>
constexpr inline auto reprof = ctvi::value<V>;

}  // namespace librepr
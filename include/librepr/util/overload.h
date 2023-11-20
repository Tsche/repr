#pragma once

namespace librepr::detail {

template <typename... Fs>
struct Overload : Fs... {
  using Fs::operator()...;
};

template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

}  // namespace librepr::detail
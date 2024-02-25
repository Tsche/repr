#pragma once

namespace librepr::util {

template <typename... Fs>
struct Overload : Fs... {
  using Fs::operator()...;
};

template <typename... Fs>
Overload(Fs...) -> Overload<Fs...>;

}  // namespace librepr::util
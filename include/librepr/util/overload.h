#pragma once

namespace librepr::detail {

template <typename... Fs>
struct Overload : Fs... {
  using Fs::operator()...;
};

}
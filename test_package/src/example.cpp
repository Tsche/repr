#include <repr>
#include <iostream>


int main() {
  auto str = repr(nullptr);
  std::cout << "repr(nullptr) = " << str << '\n';
  std::cout << std::boolalpha << "Using fmt: " << USING(REPR_FORMAT_FMT);
}
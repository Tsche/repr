#include <repr>
#include <iostream>

struct Foo{};

int main() {
  auto str = repr(nullptr);
  std::cout << "repr(nullptr) = " << str;
}

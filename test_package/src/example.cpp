#include <repr>
#include <iostream>


int main() {
  auto str = repr(nullptr);
  std::cout << "repr(nullptr) = " << str << '\n';
}
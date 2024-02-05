//* c++ gsnapshot -std=c++20 -O3
#include <repr>
#include <variant>
#include <unordered_map>
#include <iostream>

struct CustomType {
  char foo;
  unsigned bar;
  std::variant<char, bool> oof;
  std::unordered_map<int, int> zoinks;
};

int main() {
  auto object = CustomType{'f', 42U, true, {{1, 2}, {3, 4}}};
  std::cout << "Object: " << repr(object) << '\n';
  std::cout << "Type: " << librepr::code_for<CustomType>();
}

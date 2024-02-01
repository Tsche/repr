#include <repr>

enum class Foo : unsigned { a = 10, c = 11, b = 100 };

int main() {
  (void)librepr::enum_names<Foo>().size();
  (void)librepr::enum_name(Foo{10});
}
#include <gtest/gtest.h>
#include <repr>

template <typename T>
std::string get_demangled() {
  return librepr::demangle(librepr::get_mangled_name<T>());
}

TEST(demangle, fundamental) {
  EXPECT_EQ(get_demangled<int>(), "int");
  EXPECT_EQ(get_demangled<int*>(), "int*");
  EXPECT_EQ(get_demangled<int const*>(), "int const*");
  EXPECT_EQ(get_demangled<int volatile*>(), "int volatile*");
}

struct Foo {};

TEST(demangle, simple_type) {
  EXPECT_EQ(get_demangled<Foo>(), "Foo");
}

template <typename T>
struct Template {};

template <typename Required, typename... Optional>
struct VariadicTemplate {};

TEST(demangle, templated_types) {
  EXPECT_EQ(get_demangled<Template<int>>(), "Template<int>");
  EXPECT_EQ(get_demangled<Template<Foo>>(), "Template<Foo>");

  EXPECT_EQ(get_demangled<VariadicTemplate<int>>(), "VariadicTemplate<int>");
  EXPECT_EQ((get_demangled<VariadicTemplate<int, float>>()), "VariadicTemplate<int, float>");
  EXPECT_EQ((get_demangled<VariadicTemplate<int, float, Foo>>()), "VariadicTemplate<int, float, Foo>");

  EXPECT_EQ((get_demangled<VariadicTemplate<int, VariadicTemplate<int, float>, VariadicTemplate<int>>>()),
            "VariadicTemplate<int, VariadicTemplate<int, float>, VariadicTemplate<int> >");
}

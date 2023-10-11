#include <gtest/gtest.h>
#include <repr>
#include <variant>

TEST(repr, variant) {
  auto fundamental = std::variant<unsigned int, float>{42U};
  EXPECT_EQ(repr(fundamental), "std::variant<unsigned int, float>{42U}");
  fundamental = 3.14F;
  EXPECT_EQ(repr(fundamental), "std::variant<unsigned int, float>{3.14F}");
}
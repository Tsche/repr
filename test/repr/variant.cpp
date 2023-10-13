#include <gtest/gtest.h>
#include <repr>
#include <variant>

TEST(repr, variant) {
  auto fundamental = std::variant<unsigned int, float>{42U};
  EXPECT_EQ(repr(fundamental), "std::variant<unsigned int, float>{42U}");
  fundamental = 4.21F;
  EXPECT_EQ(repr(fundamental), "std::variant<unsigned int, float>{4.21F}");
}
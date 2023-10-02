#include <gtest/gtest.h>
#include <repr>

struct Unreprable{
  explicit Unreprable() = default;
};

TEST(layout, fallback) {
  EXPECT_EQ(typeinfo<Unreprable>.layout(), "Unreprable");
}
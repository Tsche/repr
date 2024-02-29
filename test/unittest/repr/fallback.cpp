#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <repr>

struct Unreprable{
  explicit Unreprable() = default;
};

TEST(fallback, unreprable) {
  using ::testing::StartsWith;
  EXPECT_THAT(repr(Unreprable{}), StartsWith("Unreprable object at"));
}
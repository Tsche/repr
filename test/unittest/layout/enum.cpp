#include <gtest/gtest.h>
#include <repr>

enum UnscopedEnum {
  FIRST = 1,
  SECOND = 2
};

enum class ScopedEnum {
  FIRST = 1,
  SECOND = 2
};

TEST(layout, enum) {
  EXPECT_EQ(librepr::layout_of<UnscopedEnum>(), "FIRST | SECOND");
  EXPECT_EQ(librepr::layout_of<ScopedEnum>(), "ScopedEnum::FIRST | ScopedEnum::SECOND");
}
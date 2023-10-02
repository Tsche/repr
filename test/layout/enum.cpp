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
  EXPECT_EQ(typeinfo<UnscopedEnum>.layout(), "FIRST | SECOND");
  EXPECT_EQ(typeinfo<ScopedEnum>.layout(), "ScopedEnum::FIRST | ScopedEnum::SECOND");
}
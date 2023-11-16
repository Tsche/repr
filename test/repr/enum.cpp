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

TEST(repr, enum) {
  EXPECT_EQ(repr(UnscopedEnum::FIRST), "FIRST");
  EXPECT_EQ(repr(ScopedEnum::FIRST), "ScopedEnum::FIRST");
#if defined _WIN32
  EXPECT_EQ(librepr::detail::denoise_name(repr(ScopedEnum::FIRST)), "ScopedEnum::FIRST");
#endif
}
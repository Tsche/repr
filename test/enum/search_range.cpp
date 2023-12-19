#include <gtest/gtest.h>
#include <librepr/enum/search.h>
#include <librepr/detail/default.h>

#include "test_enums.h"

namespace librepr::ctei::test {
TEST(enum_range, small_underlying_signed) {
  constexpr signed char minimum = -128;
  constexpr signed char maximum = 127;

  EXPECT_EQ(clamp<signed char>(-1000), minimum);
  EXPECT_EQ(clamp<signed char>(1000), maximum);

  EXPECT_EQ(Search<test_enums::unscoped::SignedChar>::reflected_min(), minimum);
  EXPECT_EQ(Search<test_enums::unscoped::SignedChar>::reflected_max(), maximum);
  EXPECT_EQ(Search<test_enums::scoped::SignedChar>::reflected_min(), minimum);
  EXPECT_EQ(Search<test_enums::scoped::SignedChar>::reflected_max(), maximum);
}

TEST(enum_range, sign_mismatch) {
  EXPECT_EQ(clamp<unsigned char>(-1000), REPR_ENUM_MIN_UNSIGNED);
  EXPECT_EQ(clamp<unsigned short>(-1000), REPR_ENUM_MIN_UNSIGNED);
  EXPECT_EQ(clamp<unsigned int>(-1000), REPR_ENUM_MIN_UNSIGNED);
  EXPECT_EQ(clamp<unsigned long>(-1000), REPR_ENUM_MIN_UNSIGNED);
  EXPECT_EQ(clamp<unsigned long long>(-1000), REPR_ENUM_MIN_UNSIGNED);
}

TEST(enum_range, largest_flag) {
  EXPECT_EQ(Search<test_enums::unscoped::Empty>::largest_flag(), -1);
  EXPECT_EQ(Search<test_enums::unscoped::Zero>::largest_flag(), 0);

  EXPECT_EQ(Search<test_enums::unscoped::UnsignedChar>::largest_flag(), 2);
  EXPECT_EQ(Search<test_enums::unscoped::UnsignedShort>::largest_flag(), 2);
  EXPECT_EQ(Search<test_enums::unscoped::Unsigned>::largest_flag(), 2);
  EXPECT_EQ(Search<test_enums::unscoped::UnsignedLong>::largest_flag(), 2);
  EXPECT_EQ(Search<test_enums::unscoped::UnsignedLongLong>::largest_flag(), 2);

  EXPECT_EQ(Search<test_enums::scoped::Empty>::largest_flag(), -1);
  EXPECT_EQ(Search<test_enums::scoped::Zero>::largest_flag(), 0);

  EXPECT_EQ(Search<test_enums::scoped::UnsignedChar>::largest_flag(), 2);
  EXPECT_EQ(Search<test_enums::scoped::UnsignedShort>::largest_flag(), 2);
  EXPECT_EQ(Search<test_enums::scoped::Unsigned>::largest_flag(), 2);
  EXPECT_EQ(Search<test_enums::scoped::UnsignedLong>::largest_flag(), 2);
  EXPECT_EQ(Search<test_enums::scoped::UnsignedLongLong>::largest_flag(), 2);
}

TEST(enum_range, custom) {
  EXPECT_EQ(Search<test_enums::unscoped::Custom>::reflected_min(), -10);
  EXPECT_EQ(Search<test_enums::unscoped::Custom>::reflected_max(), 10);
  EXPECT_EQ(Search<test_enums::scoped::Custom>::reflected_min(), -10);
  EXPECT_EQ(Search<test_enums::scoped::Custom>::reflected_max(), 10);
}

TEST(enum_range, custom_flag) {
  EXPECT_EQ(Search<test_enums::unscoped::CustomFlag>::largest_flag(), 13);
  EXPECT_EQ(Search<test_enums::unscoped::CustomFlag>::reflected_min(), REPR_ENUM_MIN_UNSIGNED);
  EXPECT_EQ(Search<test_enums::unscoped::CustomFlag>::reflected_max(), REPR_ENUM_MAX_UNSIGNED);
}

}  // namespace librepr::ctei::test
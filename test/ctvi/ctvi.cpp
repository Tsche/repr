#include <gtest/gtest.h>
#include <librepr/ctvi/ctvi.h>

TEST(ctvi, fundamental) {
  EXPECT_EQ(librepr::ctvi::value<1>, "1");
  EXPECT_EQ(librepr::ctvi::value<nullptr>, "nullptr");
  EXPECT_EQ(librepr::ctvi::value<'c'>, "'c'");
}
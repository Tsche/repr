#include <gtest/gtest.h>
#include <librepr/ctvi/ctvi.h>

TEST(ctvi, fundamental) {
  EXPECT_EQ(librepr::ctvi::value<1>.to_sv(), "1");
  EXPECT_EQ(librepr::ctvi::value<nullptr>.to_sv(), "nullptr");
  EXPECT_EQ(librepr::ctvi::value<'c'>.to_sv(), "'c'");
}
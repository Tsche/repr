#include <gtest/gtest.h>
#include <repr>

TEST(layout, containers) {
  EXPECT_EQ(librepr::layout_of<const char*[5]>(), "str[5]");
  EXPECT_EQ(librepr::layout_of<int[5]>(), "int[5]");
  // EXPECT_EQ(librepr::layout_of<const char*[]>(), "str []"); //! TODO
  EXPECT_EQ(librepr::layout_of<int[]>(), "int []");
  EXPECT_EQ(librepr::layout_of<std::vector<int>>(), "[int]");
  EXPECT_EQ((librepr::layout_of<std::map<int, int>>()), "[int -> int]");
}

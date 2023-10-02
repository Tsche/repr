#include <gtest/gtest.h>
#include <repr>

TEST(layout, containers) {
  EXPECT_EQ(typeinfo<const char*[5]>.layout(), "str[5]");
  EXPECT_EQ(typeinfo<int[5]>.layout(), "int[5]");
  EXPECT_EQ(typeinfo<const char*[]>.layout(), "[str]");
  EXPECT_EQ(typeinfo<int[]>.layout(), "[int]");
  EXPECT_EQ(typeinfo<std::vector<int>>.layout(), "[int]");
  EXPECT_EQ((typeinfo<std::map<int, int>>.layout()), "[{int, int}]");
}

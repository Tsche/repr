#include <gtest/gtest.h>
#include <repr>

TEST(initializer_list, array) {
  int integers[]        = {1, 2, 3};       //NOLINT
  char const* strings[] = {"1", "2", "3"}; //NOLINT

  EXPECT_EQ(repr(integers), "{1, 2, 3}");
  EXPECT_EQ(repr(strings), R"({"1", "2", "3"})");
}

TEST(initializer_list, vector) {
  EXPECT_EQ(repr(std::vector<int>{1, 2, 3}), "std::vector<int>{1, 2, 3}");
}

TEST(initializer_list, map) {
  EXPECT_EQ(repr(std::map<int, int>{{1, 6}, {2, 5}, {3, 4}}), "std::map<int, int>{{1, 6}, {2, 5}, {3, 4}}");
}

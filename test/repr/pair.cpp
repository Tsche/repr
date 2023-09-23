#include <gtest/gtest.h>
#include <repr>

TEST(pair, pair) {
  EXPECT_EQ(repr(std::pair<int, int>{1, 2}), "std::pair<int, int>{1, 2}");
  auto const foo = std::pair<int, int>{1, 2};
  EXPECT_EQ(repr(foo), "std::pair<int, int>{1, 2}");
}

TEST(pair, pair_of_pairs) {
  auto obj = std::pair<std::pair<int, char>, std::pair<char, int>>{{1, '2'}, {'3', 4}};
  EXPECT_EQ(repr(obj), "std::pair<std::pair<int, char>, std::pair<char, int>>{{1, '2'}, {'3', 4}]");
}
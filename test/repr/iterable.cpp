#include <gtest/gtest.h>
#include <repr>

TEST(iterable, array) {
  int integers[]        = {1, 2, 3};
  char const* strings[] = {"1", "2", "3"};

  EXPECT_EQ(repr(integers), "{1, 2, 3}");
  EXPECT_EQ(repr(strings), R"({"1", "2", "3"})");
}

TEST(iterable, vector) {
  EXPECT_EQ(repr(std::vector<int>{1, 2, 3}), "std::vector<int>{1, 2, 3}");
}

TEST(iterable, map) {
  EXPECT_EQ(repr(std::map<int, int>{{1, 6}, {2, 5}, {3, 4}}), "std::map<int, int>{{1, 6}, {2, 5}, {3, 4}}");
}

TEST(iterable, unordered_map) {
  // TODO
  EXPECT_EQ(repr(std::unordered_map<int, int>{{1, 6}, {2, 5}, {3, 4}}),
            "std::unordered_map<int, int>{{3, 4}, {2, 5}, {1, 6}}");
}

TEST(iterable, pair) {
  EXPECT_EQ(repr(std::pair<int, int>{1, 2}), "std::pair<int, int>{1, 2}");
  auto const foo = std::pair<int, int>{1, 2};
  EXPECT_EQ(repr(foo), "std::pair<int, int>{1, 2}");
}
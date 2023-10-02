#include <gtest/gtest.h>
#include <repr>

TEST(layout, pair) {
  using pair = std::pair<int, int>;
  using pair_of_pairs = std::pair<std::pair<int, int>, std::pair<int, int>>;
  EXPECT_EQ(typeinfo<pair>.layout(), "{int, int}");
  EXPECT_EQ(typeinfo<pair_of_pairs>.layout(), "{{int, int}, {int, int}}");
}

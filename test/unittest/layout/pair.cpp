#include <gtest/gtest.h>
#include <repr>

TEST(layout, pair) {
  using pair = std::pair<int, int>;
  using pair_of_pairs = std::pair<std::pair<int, int>, std::pair<int, int>>;
  EXPECT_EQ(librepr::layout_of<pair>(), "{int, int}");
  EXPECT_EQ(librepr::layout_of<pair_of_pairs>(), "{{int, int}, {int, int}}");
}

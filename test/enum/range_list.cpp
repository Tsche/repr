#include <concepts>

#include <gtest/gtest.h>
#include <librepr/enum/range_list.h>

namespace librepr::ctei::test {

TEST(range_list, add) {
  EXPECT_TRUE((std::same_as<RangeList<>::add<0>, RangeList<Range<0>>>));
  EXPECT_TRUE((std::same_as<RangeList<Range<0>>::add<1>, RangeList<Range<0, 1>>>));
  EXPECT_TRUE((std::same_as<RangeList<Range<0, 1>>::add<4>, RangeList<Range<4>, Range<0, 1>>>));
  EXPECT_TRUE((std::same_as<RangeList<Range<4>, Range<0, 1>>::add<5>, RangeList<Range<4, 5>, Range<0, 1>>>));
}

TEST(range_list, add_range) {
  EXPECT_TRUE((std::same_as<RangeList<>::add_range<Range<0>>, RangeList<Range<0>>>));
  EXPECT_TRUE((std::same_as<RangeList<Range<0>>::add_range<Range<1>>, RangeList<Range<0, 1>>>));
  
  EXPECT_TRUE((std::same_as<RangeList<Range<0>>::add_range<Range<2>>, RangeList<Range<2>, Range<0>>>));
  EXPECT_TRUE((std::same_as<RangeList<Range<0>>::add_range<Range<2, 5>>, RangeList<Range<2, 5>, Range<0>>>));

  EXPECT_TRUE((std::same_as<RangeList<Range<4>, Range<0, 1>>::add_range<Range<5>>, RangeList<Range<4, 5>, Range<0, 1>>>));
}

TEST(range_list, binary_powers) {
  EXPECT_TRUE((RangeList<Range<0>>::is_binary_powers()));
  EXPECT_TRUE((RangeList<Range<0, 1>>::is_binary_powers()));
  EXPECT_TRUE((RangeList<Range<0, 2>>::is_binary_powers()));
  EXPECT_TRUE((RangeList<Range<0, 2>, Range<4>>::is_binary_powers()));
  EXPECT_TRUE((RangeList<Range<4>, Range<8>, Range<16>>::is_binary_powers()));
  EXPECT_TRUE((RangeList<Range<4>, Range<16>>::is_binary_powers()));
  EXPECT_TRUE((RangeList<Range<0>, Range<16>>::is_binary_powers()));
  
  EXPECT_TRUE((RangeList<>::is_binary_powers()));

  EXPECT_FALSE((RangeList<Range<3>>::is_binary_powers()));
  EXPECT_FALSE((RangeList<Range<0, 3>>::is_binary_powers()));
  EXPECT_FALSE((RangeList<Range<0, 2>, Range<7>>::is_binary_powers()));
}

}
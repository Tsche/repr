#include <concepts>

#include <gtest/gtest.h>
#include <librepr/enum/search.h>
#include "test_enums.h"

namespace librepr::ctei::test {

TEST(range, contains) {
  EXPECT_FALSE((Range<1, 3>::contains(-1)));  // out of range
  EXPECT_FALSE((Range<1, 3>::contains(0))); // out of range
  EXPECT_TRUE((Range<1, 3>::contains(1)));
  EXPECT_TRUE((Range<1, 3>::contains(2)));
  EXPECT_TRUE((Range<1, 3>::contains(3)));
  EXPECT_FALSE((Range<1, 3>::contains(4))); // out of range

  EXPECT_TRUE((Range<0U, 1U>::contains(0U)));
  EXPECT_TRUE((Range<0U, 1U>::contains(1U)));
  EXPECT_FALSE((Range<0U, 1U>::contains(2U)));  // out of range

  EXPECT_TRUE((Range<0U, 0U>::contains(0U)));
}

TEST(range, is_binary_powers) {
  EXPECT_TRUE((Range<0U, 1U>::is_binary_powers()));
  EXPECT_TRUE((Range<0U, 2U>::is_binary_powers()));
  EXPECT_TRUE((Range<4U, 4U>::is_binary_powers()));
  EXPECT_TRUE((Range<8U, 8U>::is_binary_powers()));

  EXPECT_FALSE((Range<1U, 3U>::is_binary_powers()));  // range includes not power of 2
  EXPECT_FALSE((Range<4U, 5U>::is_binary_powers()));  // range includes not power of 2
  EXPECT_FALSE((Range<5U, 5U>::is_binary_powers()));  // min is not power of 2
}

TEST(range, expand) {
  EXPECT_TRUE((std::same_as<Range<0>::template expand<>, Range<0, 1>>));
  EXPECT_TRUE((std::same_as<Range<0, 5>::template expand<>, Range<0, 6>>));
  EXPECT_TRUE((std::same_as<Range<0>::template expand<10>, Range<0, 10>>));
  EXPECT_TRUE((std::same_as<Range<0, 5>::template expand<10>, Range<0, 15>>));
}

TEST(range, get) {
  EXPECT_EQ(Range<0>::get<0>, 0);
  EXPECT_EQ((Range<0, 5>::get<5>), 5);
  EXPECT_EQ((Range<10, 15>::get<2>), 12);
}

TEST(range, size) {
  EXPECT_EQ(Range<0>::size, 1);
  EXPECT_EQ(Range<10>::size, 1);
  EXPECT_EQ((Range<0, 5>::size), 6);
  EXPECT_EQ((Range<0, 1>::size), 2);
  EXPECT_EQ((Range<10, 15>::size), 6);
  EXPECT_EQ((Range<10, 11>::size), 2);
}

TEST(range, min_max) {
  EXPECT_EQ(Range<0>::min, 0);
  EXPECT_EQ(Range<10>::min, 10);
  EXPECT_EQ((Range<0, 5>::min), 0);
  EXPECT_EQ((Range<0, 1>::min), 0);
  EXPECT_EQ((Range<10, 15>::min), 10);
  EXPECT_EQ((Range<10, 11>::min), 10);

  EXPECT_EQ(Range<0>::max, 0);
  EXPECT_EQ(Range<10>::max, 10);
  EXPECT_EQ((Range<0, 5>::max), 5);
  EXPECT_EQ((Range<0, 1>::max), 1);
  EXPECT_EQ((Range<10, 15>::max), 15);
  EXPECT_EQ((Range<10, 11>::max), 11);
}

}  // namespace librepr::ctei::test
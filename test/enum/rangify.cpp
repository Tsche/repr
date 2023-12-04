#include <array>
#include <concepts>

#include <gtest/gtest.h>
#include <librepr/enum/search.h>
#include "test_enums.h"

namespace librepr::ctei::test {
inline constexpr auto empty = std::array<bool, 0>{};
inline constexpr auto all_false = std::array{false, false, false, false, false};
inline constexpr auto all_true = std::array{true, true, true, true, true};
inline constexpr auto one_range = std::array{true, true, false, false, false};
inline constexpr auto two_ranges = std::array{true, true, false, true, true};

TEST(enum, chunk_find_first){
  EXPECT_EQ(find_first(empty.data(), 0, empty.size(), true), empty.size());
  EXPECT_EQ(find_first(empty.data(), 0, empty.size(), false), empty.size());
  
  EXPECT_EQ(find_first(all_false.data(), 0, all_false.size(), true), all_false.size());
  EXPECT_EQ(find_first(all_false.data(), 0, all_false.size(), false), 0);

  EXPECT_EQ(find_first(all_true.data(), 0, all_true.size(), true), 0);
  EXPECT_EQ(find_first(all_true.data(), 0, all_true.size(), false), all_true.size());

  EXPECT_EQ(find_first(one_range.data(), 0, one_range.size(), true), 0);
  EXPECT_EQ(find_first(one_range.data(), 0, one_range.size(), false), 2);
  EXPECT_EQ(find_first(one_range.data(), 3, one_range.size(), true), one_range.size());

  EXPECT_EQ(find_first(two_ranges.data(), 0, two_ranges.size(), true), 0);
  EXPECT_EQ(find_first(two_ranges.data(), 0, two_ranges.size(), false), 2);
  EXPECT_EQ(find_first(two_ranges.data(), 3, two_ranges.size(), true), 3);
  EXPECT_EQ(find_first(two_ranges.data(), 3, two_ranges.size(), false), two_ranges.size());
}

TEST(enum, rangify){
  EXPECT_TRUE((std::same_as<decltype(rangify<empty, 0, RangeList<>>()), RangeList<>>));
  EXPECT_TRUE((std::same_as<decltype(rangify<empty, 10, RangeList<>>()), RangeList<>>));

  EXPECT_TRUE((std::same_as<decltype(rangify<all_false, 0, RangeList<>>()), RangeList<>>));
  EXPECT_TRUE((std::same_as<decltype(rangify<all_false, 10, RangeList<>>()), RangeList<>>));
}

TEST(enum, rangify_one_range) {
  EXPECT_TRUE((std::same_as<decltype(rangify<all_true, 0, RangeList<>>()), RangeList<Range<0, 4>>>));
  EXPECT_TRUE((std::same_as<decltype(rangify<all_true, 10, RangeList<>>()), RangeList<Range<10, 14>>>));

  EXPECT_TRUE((std::same_as<decltype(rangify<one_range, 0, RangeList<>>()), RangeList<Range<0, 1>>>));
  EXPECT_TRUE((std::same_as<decltype(rangify<one_range, 10, RangeList<>>()), RangeList<Range<10, 11>>>));
}

TEST(enum, rangify_two_ranges) {
  EXPECT_TRUE((std::same_as<decltype(rangify<two_ranges, 0, RangeList<>>()), RangeList<Range<0, 1>, Range<3, 4>>>));
  EXPECT_TRUE((std::same_as<decltype(rangify<two_ranges, 10, RangeList<>>()), RangeList<Range<10, 11>, Range<13, 14>>>));
}
}
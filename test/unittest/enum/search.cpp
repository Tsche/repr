#include <gtest/gtest.h>
#include <librepr/enum/search.h>
#include "test_enums.h"

namespace librepr::ctei::test {

TEST(enum_search, recursive){
  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Empty>::search_recursive<0, 64>, RangeList<>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Empty>::search_recursive<0, 64>, RangeList<>>));

  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Unsigned>::search_recursive<0, 64>, 
                            RangeList<Range<5U, 6U>, Range<1U, 3U>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Unsigned>::search_recursive<0, 64>, 
                            RangeList<Range<5U, 6U>, Range<1U, 3U>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Signed>::search_recursive<-32, 64>, 
                            RangeList<Range<-2, 2>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Signed>::search_recursive<-32, 64>, 
                            RangeList<Range<-2, 2>>>));
}

TEST(enum_search, chunked){
  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Empty>::search_chunked<0, 64>, RangeList<>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Empty>::search_chunked<0, 64>, RangeList<>>));

  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Unsigned>::search_chunked<0, 64>, 
                            RangeList<Range<5U, 6U>, Range<1U, 3U>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Unsigned>::search_chunked<0, 64>, 
                            RangeList<Range<5U, 6U>, Range<1U, 3U>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Signed>::search_chunked<-32, 64>, 
                            RangeList<Range<-2, 2>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Signed>::search_chunked<-32, 64>, 
                            RangeList<Range<-2, 2>>>));
}

TEST(enum_search, fast){
  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Empty>::search_fast<0, 64>, RangeList<>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Empty>::search_fast<0, 64>, RangeList<>>));

  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Unsigned>::search_fast<0, 64>, 
                            RangeList<Range<5U, 6U>, Range<1U, 3U>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Unsigned>::search_fast<0, 64>, 
                            RangeList<Range<5U, 6U>, Range<1U, 3U>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::scoped::Signed>::search_fast<-32, 64>, 
                            RangeList<Range<-2, 2>>>));
  EXPECT_TRUE((std::same_as<Search<test_enums::unscoped::Signed>::search_fast<-32, 64>, 
                            RangeList<Range<-2, 2>>>));
}

}
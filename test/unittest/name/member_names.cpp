#include <gtest/gtest.h>
#include <librepr/name/member.h>

struct NameTestAggregate {
  char a;
  int test_array[5];
  struct { int foo; } inner_aggregate;
  std::string a_very_long_member_name_with_lots_of_words_and_also_numbers_234897289_O;
};

namespace librepr::detail::test {
TEST(names, member_names) {
  auto names = librepr::member_names<NameTestAggregate>;
  EXPECT_EQ(names.size(), 4);
  EXPECT_EQ(names[0], "a");
  EXPECT_EQ(names[1], "test_array");
  EXPECT_EQ(names[2], "inner_aggregate");
  EXPECT_EQ(names[3], "a_very_long_member_name_with_lots_of_words_and_also_numbers_234897289_O");
}

TEST(names, member_name) {
  EXPECT_EQ((member_name<NameTestAggregate, 0>), "a");
  EXPECT_EQ((member_name<NameTestAggregate, 1>), "test_array");
  EXPECT_EQ((member_name<NameTestAggregate, 2>), "inner_aggregate");
  EXPECT_EQ((member_name<NameTestAggregate, 3>), "a_very_long_member_name_with_lots_of_words_and_also_numbers_234897289_O");
}

}
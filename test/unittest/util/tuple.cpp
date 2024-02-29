#include <gtest/gtest.h>
#include <librepr/util/collections/tuple.h>

struct TupleTest : public ::testing::Test {
  librepr::Tuple<> empty{};
  librepr::Tuple<int, char, float> values{3, 'b', 1.23F};
  int dummy_a = 3;
  char dummy_b = 'b';
  float dummy_c = 1.23F;
  librepr::Tuple<int&, char const&, float&> references{dummy_a, dummy_b, dummy_c};
};

TEST_F(TupleTest, size){
  EXPECT_EQ(std::tuple_size_v<decltype(empty)>, 0);
  EXPECT_EQ(std::tuple_size_v<decltype(values)>, 3);
  EXPECT_EQ(std::tuple_size_v<decltype(references)>, 3);
}

TEST_F(TupleTest, element){
  EXPECT_TRUE((std::same_as<std::tuple_element_t<0, decltype(values)>, int>));
  EXPECT_TRUE((std::same_as<std::tuple_element_t<1, decltype(values)>, char>));
  EXPECT_TRUE((std::same_as<std::tuple_element_t<2, decltype(values)>, float>));

  EXPECT_TRUE((std::same_as<std::tuple_element_t<0, decltype(references)>, int&>));
  EXPECT_TRUE((std::same_as<std::tuple_element_t<1, decltype(references)>, char const&>));
  EXPECT_TRUE((std::same_as<std::tuple_element_t<2, decltype(references)>, float&>));
}

TEST_F(TupleTest, get){
  EXPECT_EQ(get<0>(values), 3);
  EXPECT_EQ(get<1>(values), 'b');
  EXPECT_EQ(get<2>(values), 1.23F);
  EXPECT_EQ(get<0>(references), 3);
  EXPECT_EQ(get<1>(references), 'b');
  EXPECT_EQ(get<2>(references), 1.23F);
}
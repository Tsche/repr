#include <gtest/gtest.h>
#include <librepr/util/collections/reftuple.h>

struct RefTupleTest : public ::testing::Test {
  librepr::RefTuple<> empty{};
  int dummy_a = 2;
  char dummy_b = 'c';
  float dummy_c = 1.34F;
  librepr::RefTuple<int&, char const&, float&> references{dummy_a, dummy_b, dummy_c};
};

TEST_F(RefTupleTest, size){
  EXPECT_EQ(std::tuple_size_v<decltype(empty)>, 0);
  EXPECT_EQ(std::tuple_size_v<decltype(references)>, 3);
}

TEST_F(RefTupleTest, element){
  EXPECT_TRUE((std::same_as<std::tuple_element_t<0, decltype(references)>, int&>));
  EXPECT_TRUE((std::same_as<std::tuple_element_t<1, decltype(references)>, char const&>));
  EXPECT_TRUE((std::same_as<std::tuple_element_t<2, decltype(references)>, float&>));
}

TEST_F(RefTupleTest, get){
  EXPECT_EQ(get<0>(references), 2);
  EXPECT_EQ(get<1>(references), 'c');
  EXPECT_EQ(get<2>(references), 1.34F);
}
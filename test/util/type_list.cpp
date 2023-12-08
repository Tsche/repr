#include <concepts>
#include <type_traits>
#include <gtest/gtest.h>
#include <librepr/util/list.h>

#define COMP_ASSERT(expr) EXPECT_TRUE(( requires {requires (expr);}))

namespace librepr::test {


using small_list = TypeList<int, char, float, double>;
// NOLINTBEGIN
using big_list = TypeList<int, char, float, double, int, int, int, int, 
                           int, char, float, double, int, int, int, int,
                           int, char, float, double, int, int, int, int,
                           int, char, float, double, int, int, int, int>;
// NOLINTEND

TEST(type_list, map){ }

TEST(type_list, get){
  COMP_ASSERT((std::same_as<small_list::get<0>, int>));
  COMP_ASSERT((std::same_as<small_list::get<1>, char>));
  COMP_ASSERT((std::same_as<small_list::get<2>, float>));
  COMP_ASSERT((std::same_as<small_list::get<3>, double>));

  COMP_ASSERT((std::same_as<big_list::get<0>, int>));
  COMP_ASSERT((std::same_as<big_list::get<4>, int>));
  COMP_ASSERT((std::same_as<big_list::get<15>, int>));
  COMP_ASSERT((std::same_as<big_list::get<16>, int>));
  COMP_ASSERT((std::same_as<big_list::get<17>, char>));
  COMP_ASSERT((std::same_as<big_list::get<31>, int>));
}

TEST(type_list, head){
  COMP_ASSERT((std::same_as<small_list::head<1>, TypeList<int, char>>));
  COMP_ASSERT((std::same_as<small_list::head<0>, TypeList<int>>));
  COMP_ASSERT((std::same_as<small_list::head<3>, TypeList<int, char, float, double>>));

  COMP_ASSERT((std::same_as<big_list::head<3>, TypeList<int, char, float, double>>));
  COMP_ASSERT((std::same_as<big_list::head<15>, TypeList<int, char, float, double, int, int, int, int, 
                                                          int, char, float, double, int, int, int, int>>));
}

TEST(type_list, tail){
  COMP_ASSERT((std::same_as<small_list::tail<1>, TypeList<float, double>>));
  COMP_ASSERT((std::same_as<small_list::tail<0>, TypeList<char, float, double>>));
  COMP_ASSERT((std::same_as<small_list::tail<3>, TypeList<>>));

  COMP_ASSERT((std::same_as<big_list::tail<29>, TypeList<int, int>>));
  COMP_ASSERT((std::same_as<big_list::tail<15>, TypeList<int, char, float, double, int, int, int, int,
                                                          int, char, float, double, int, int, int, int>>));
}

using big_list = TypeList<int, char, float, double, int, int, int, int, 
                           int, char, float, double, int, int, int, int,
                           int, char, float, double, int, int, int, int,
                           int, char, float, double, int, int, int, int>;

TEST(type_list, split){
  EXPECT_TRUE((std::same_as<small_list::split<1>, TypeList<TypeList<int, char>, TypeList<float, double>>>));
  EXPECT_TRUE((std::same_as<big_list::split<23>, TypeList<TypeList<int, char, float, double, int, int, int, int, 
                                                                     int, char, float, double, int, int, int, int,
                                                                     int, char, float, double, int, int, int, int>,
                                                           TypeList<int, char, float, double, int, int, int, int>>>));
}

TEST(type_list, mutate){ 
  EXPECT_TRUE((std::same_as<TypeList<>::append<int>, TypeList<int>>));
  EXPECT_TRUE((std::same_as<TypeList<int>::append<char>, TypeList<int, char>>));

  EXPECT_TRUE((std::same_as<TypeList<>::prepend<int>, TypeList<int>>));
  EXPECT_TRUE((std::same_as<TypeList<int>::prepend<char>, TypeList<char, int>>));
}

template <typename...>
struct DummyList{};

TEST(type_list, rebox){
  EXPECT_TRUE((std::same_as<TypeList<int, char, float>::to<DummyList>, DummyList<int, char, float>>));
  EXPECT_TRUE((std::same_as<pack::rebox<TypeList<int, char, float>, DummyList>, DummyList<int, char, float>>));
  EXPECT_TRUE((std::same_as<pack::rebox<DummyList<int, char, float>, TypeList>, TypeList<int, char, float>>));
}

TEST(type_list, invoke){ 
  auto count_small = 0;
  auto count_big = 0;
  small_list::invoke([&count_small]<typename... Ts>(){count_small = sizeof...(Ts);});
  big_list::invoke([&count_big]<typename... Ts>(){count_big = sizeof...(Ts);});
  EXPECT_EQ(count_small, 4);
  EXPECT_EQ(count_big, 32);
}

TEST(type_list, for_each){
  auto count_small = 0;
  auto count_big = 0;
  small_list::for_each([&count_small]<typename>(){++count_small;});
  big_list::for_each([&count_big]<typename>(){++count_big;});
  EXPECT_EQ(count_small, 4);
  EXPECT_EQ(count_big, 32);
}
TEST(type_list, enumerate){ 
  auto count_small = 0;
  auto count_big = 0;
  small_list::enumerate([&count_small]<typename>(std::size_t index){
    EXPECT_EQ(count_small, index);
    ++count_small;
  });
  big_list::enumerate([&count_big]<typename>(std::size_t index){
    EXPECT_EQ(count_big, index);
    ++count_big;
  });
  EXPECT_EQ(count_small, 4);
  EXPECT_EQ(count_big, 32);

  count_small = 0;
  count_big = 0;
  small_list::enumerate([&count_small]<typename, std::size_t index>(){
    EXPECT_EQ(count_small, index);
    ++count_small;
  });
  big_list::enumerate([&count_big]<typename, std::size_t index>(){
    EXPECT_EQ(count_big, index);
    ++count_big;
  });
  EXPECT_EQ(count_small, 4);
  EXPECT_EQ(count_big, 32);
}
/*
static_assert(std::same_as<TypeList<>::map<std::type_identity>, TypeList<>>);
static_assert(std::same_as<TypeList<int, char>::map<std::type_identity>,
                           TypeList<std::type_identity<int>, std::type_identity<char>>>);

static_assert(std::same_as<TypeList<>::map_t<std::type_identity>, TypeList<>>);
static_assert(std::same_as<TypeList<int, char>::map_t<std::type_identity>, TypeList<int, char>>);

static_assert(std::same_as<TypeList<int, char, float>::get<0>, int>);
static_assert(std::same_as<TypeList<int, char, float>::get<1>, char>);
static_assert(std::same_as<TypeList<int, char, float>::get<2>, float>);

/*static_assert(std::same_as<TypeList<int, char, float>::slice<0, 1>, TypeList<int>>);
static_assert(std::same_as<TypeList<int, char, float>::slice<1>, TypeList<char, float>>);
static_assert(std::same_as<TypeList<int, char, float>::slice<1, 2>, TypeList<char>>);
static_assert(std::same_as<TypeList<int, char, float>::slice<1, 1>, TypeList<>>);* /

static_assert(std::same_as<TypeList<int, char, float>::split<3>, TypeList<TypeList<int, char, float>, TypeList<>>>);
static_assert(std::same_as<TypeList<int, char, float>::split<1>, TypeList<TypeList<int>, TypeList<char, float>>>);
static_assert(std::same_as<TypeList<int, char, float>::split<2>, TypeList<TypeList<int, char>, TypeList<float>>>);
static_assert(std::same_as<TypeList<int, char, float>::split<0>, TypeList<TypeList<>, TypeList<int, char, float>>>);

static_assert(std::same_as<TypeList<>::append<float>, TypeList<float>>);
static_assert(std::same_as<TypeList<>::prepend<float>, TypeList<float>>);
static_assert(std::same_as<TypeList<int, char>::append<float>, TypeList<int, char, float>>);
static_assert(std::same_as<TypeList<int, char>::prepend<float>, TypeList<float, int, char>>);

static_assert(std::same_as<TypeList<>::from_tuple<std::tuple<>>, TypeList<>>);
static_assert(std::same_as<TypeList<>::from_tuple<std::tuple<int, char, float>>, TypeList<int, char, float>>);
static_assert(std::same_as<TypeList<>::to<std::tuple>, std::tuple<>>);
static_assert(std::same_as<TypeList<int, char, float>::to<std::tuple>, std::tuple<int, char, float>>);

static_assert(std::same_as<TypeList<int, char>::map<std::type_identity>,
                           TypeList<std::type_identity<int>, std::type_identity<char>>>);

static_assert(std::same_as<TypeList<int, char>::map_t<std::type_identity>, TypeList<int, char>>);
*/
}  // namespace librepr

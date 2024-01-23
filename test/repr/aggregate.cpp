#include <gtest/gtest.h>
#include <repr>
#include <map>

struct TestAggregate {
  int a; 
  char b;
  double c;
  unsigned long long d;
};

TEST(aggregate, fundamental) {
  EXPECT_EQ(repr(TestAggregate{1, 'c', 3.3, 5ULL}), "TestAggregate{.a=1, .b='c', .c=3.3, .d=5ULL}");
}

TEST(aggregate, map_of_aggregates) {
  auto map = std::map<unsigned int, TestAggregate>{{5U, {2, 'b', 1.2, 3ULL}}, {8U, {1, 'a', 1.0, 0ULL}}};
  EXPECT_EQ(repr(map), "std::map<unsigned int, TestAggregate>{{5U, {.a=2, .b='b', .c=1.2, .d=3ULL}}, {8U, {.a=1, .b='a', .c=1.0, .d=0ULL}}}");
}

struct TestNestedAggregate {
  TestAggregate foo;
  int a;
};

TEST(aggregate, nested) {
  auto nested = TestNestedAggregate{{1, 'c', 3.3, 5ULL}, 3};
  EXPECT_EQ(repr(nested), "TestNestedAggregate{.foo={.a=1, .b='c', .c=3.3, .d=5ULL}, .a=3}");
}
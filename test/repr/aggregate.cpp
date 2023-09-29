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
  EXPECT_EQ(repr(TestAggregate{1, 'c', 3.3, 5ULL}), "TestAggregate{1, 'c', 3.3, 5ULL}");
}

TEST(aggregate, map_of_aggregates) {
  auto map = std::map<unsigned int, TestAggregate>{{5U, {2, 'b', 1.2, 3ULL}}, {8U, {1, 'a', 1.0, 0ULL}}};
  EXPECT_EQ(repr(map), "std::map<unsigned int, TestAggregate>{{5U, {2, 'b', 1.2, 3ULL}}, {8U, {1, 'a', 1.0, 0ULL}}}");
}

struct TestNestedAggregate {
  TestAggregate foo;
  int a;
};

TEST(aggregate, nested) {
  auto nested = TestNestedAggregate{{1, 'c', 3.3, 5ULL}, 3};
  EXPECT_EQ(repr(nested), "TestNestedAggregate{{1, 'c', 3.3, 5ULL}, 3}");
}
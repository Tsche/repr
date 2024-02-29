#include <gtest/gtest.h>
#include <repr>
#include <map>

struct TestAggregate {
  int a; 
  char b;
  double c;
  unsigned long long d;
};

struct TestNestedAggregate {
  TestAggregate foo;
  int a;
};

TEST(layout, aggregate) {
  EXPECT_EQ(librepr::layout_of<TestAggregate>(), "{int, char, double, unsigned long long}");
  EXPECT_EQ(librepr::layout_of<TestNestedAggregate>(), "{{int, char, double, unsigned long long}, int}");
}

TEST(layout, map_of_aggregates) {
  using map_type = std::map<unsigned int, TestAggregate>;
  EXPECT_EQ(librepr::layout_of<map_type>(), "[unsigned int -> {int, char, double, unsigned long long}]");
}

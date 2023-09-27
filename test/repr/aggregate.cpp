#include <gtest/gtest.h>
#include <repr>


struct TestAggregate {
  int a; 
  char b;
  double c;
  unsigned long long d;
};

TEST(aggregate, fundamental) {
  EXPECT_EQ(repr(TestAggregate{1, 'c', 3.3, 5ULL}), "TestAggregate{1, 'c', 3.3, 5ULL}");
}
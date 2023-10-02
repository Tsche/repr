#include <gtest/gtest.h>
#include <repr>

TEST(layout, nullptr){
  // TODO this is not nice
  EXPECT_EQ(typeinfo<decltype(nullptr)>.layout(), "decltype(nullptr)");
}

struct Test {};
TEST(layout, pointer) {
  EXPECT_EQ(typeinfo<int*>.layout(), "int*");
  EXPECT_EQ(typeinfo<void*>.layout(), "void*");
  EXPECT_EQ(typeinfo<testing::Test*>.layout(), "testing::Test*");
}
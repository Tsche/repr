#include <gtest/gtest.h>
#include <repr>

TEST(layout, nullptr){
  // TODO this is not nice
  EXPECT_EQ(librepr::layout_of<decltype(nullptr)>(), "decltype(nullptr)");
}

struct Test {};
TEST(layout, pointer) {
  EXPECT_EQ(librepr::layout_of<int*>(), "int*");
  EXPECT_EQ(librepr::layout_of<void*>(), "void*");
  EXPECT_EQ(librepr::layout_of<testing::Test*>(), "testing::Test*");
}
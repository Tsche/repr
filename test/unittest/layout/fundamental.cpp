#include <gtest/gtest.h>
#include <repr>

TEST(layout, fundamental) {
  EXPECT_EQ(librepr::layout_of<bool>(), "bool");
  EXPECT_EQ(librepr::layout_of<char>(), "char");
  EXPECT_EQ(librepr::layout_of<short>(), "short");
  EXPECT_EQ(librepr::layout_of<unsigned short>(), "unsigned short");
  EXPECT_EQ(librepr::layout_of<int>(), "int");
  EXPECT_EQ(librepr::layout_of<unsigned int>(), "unsigned int");
  EXPECT_EQ(librepr::layout_of<long>(), "long");
  EXPECT_EQ(librepr::layout_of<unsigned long>(), "unsigned long");
  EXPECT_EQ(librepr::layout_of<long long>(), "long long");
  EXPECT_EQ(librepr::layout_of<unsigned long long>(), "unsigned long long");
  EXPECT_EQ(librepr::layout_of<float>(), "float");
  EXPECT_EQ(librepr::layout_of<double>(), "double");
  EXPECT_EQ(librepr::layout_of<long double>(), "long double");
}
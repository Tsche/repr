#include <gtest/gtest.h>
#include <repr>

TEST(layout, fundamental) {
  EXPECT_EQ(typeinfo<bool>.layout(), "bool");
  EXPECT_EQ(typeinfo<char>.layout(), "char");
  EXPECT_EQ(typeinfo<short>.layout(), "short");
  EXPECT_EQ(typeinfo<unsigned short>.layout(), "unsigned short");
  EXPECT_EQ(typeinfo<int>.layout(), "int");
  EXPECT_EQ(typeinfo<unsigned int>.layout(), "unsigned int");
  EXPECT_EQ(typeinfo<long>.layout(), "long");
  EXPECT_EQ(typeinfo<unsigned long>.layout(), "unsigned long");
  EXPECT_EQ(typeinfo<long long>.layout(), "long long");
  EXPECT_EQ(typeinfo<unsigned long long>.layout(), "unsigned long long");
  EXPECT_EQ(typeinfo<float>.layout(), "float");
  EXPECT_EQ(typeinfo<double>.layout(), "double");
  EXPECT_EQ(typeinfo<long double>.layout(), "long double");
}
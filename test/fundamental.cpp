#include <gtest/gtest.h>
#include <repr>


TEST(fundamental, bool) {
  EXPECT_EQ(repr(true), "true");
  EXPECT_EQ(repr(false), "false");
}

TEST(fundamental, short) {
  EXPECT_EQ(repr(short(0)), "0");
  EXPECT_EQ(repr(short(1)), "1");
  EXPECT_EQ(repr((unsigned short)0), "0");
  EXPECT_EQ(repr((unsigned short)1), "1");
}

TEST(fundamental, int) {
  EXPECT_EQ(repr(0), "0");
  EXPECT_EQ(repr(1), "1");
  EXPECT_EQ(repr(0U), "0U");
  EXPECT_EQ(repr(1U), "1U");
}

TEST(fundamental, long) {
  EXPECT_EQ(repr(0L), "0L");
  EXPECT_EQ(repr(1L), "1L");
  EXPECT_EQ(repr(0UL), "0UL");
  EXPECT_EQ(repr(1UL), "1UL");
}

TEST(fundamental, long_long) {
  EXPECT_EQ(repr(0LL), "0LL");
  EXPECT_EQ(repr(1LL), "1LL");
  EXPECT_EQ(repr(0ULL), "0ULL");
  EXPECT_EQ(repr(1ULL), "1ULL");
}

TEST(fundamental, float) {
  EXPECT_EQ(repr(0.0F), "0.0F");
  EXPECT_EQ(repr(1.0F), "1.0F");

  EXPECT_EQ(repr(0.1F), "0.1F");
  EXPECT_EQ(repr(3.141592F), "3.141592F");
}

TEST(fundamental, double) {
  EXPECT_EQ(repr(0.0), "0.0");
  EXPECT_EQ(repr(1.0), "1.0");

  EXPECT_EQ(repr(0.1), "0.1");
  EXPECT_EQ(repr(3.141592653589793), "3.141592653589793");
}

TEST(fundamental, long_double) {
  EXPECT_EQ(repr(0.0L), "0.0L");
  EXPECT_EQ(repr(1.0L), "1.0L");

  EXPECT_EQ(repr(0.1L), "0.1L");
  EXPECT_EQ(repr(3.141592653589793238L), "3.141592653589793238L");
}

TEST(fundamental, char) {
  EXPECT_EQ(repr('a'), "'a'");
  EXPECT_EQ(repr((char)0xff), "'\xFF'");
}
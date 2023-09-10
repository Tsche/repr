#include <gtest/gtest.h>
#include <repr>

TEST(pointer, nullptr) {
  EXPECT_EQ(repr(nullptr), "nullptr");
}

struct Test{};

TEST(pointer, pointer) {
  EXPECT_EQ(repr((int*)0x123ff), "(int*)0x123ff");
  EXPECT_EQ(repr((testing::Test*)0x123ff), "(testing::Test*)0x123ff");
  EXPECT_EQ(repr((void*)0x123ff), "(void*)0x123ff");
}

TEST(pointer, char_pointer) {
  EXPECT_EQ(repr("foo"), "\"foo\"");
}
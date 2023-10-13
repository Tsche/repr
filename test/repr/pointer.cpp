#include <gtest/gtest.h>
#include <repr>

TEST(pointer, nullptr) {
  EXPECT_EQ(repr(nullptr), "nullptr");
}

struct Test {};

#if defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
  #pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
//NOLINTBEGIN

TEST(pointer, pointer) {
  auto int_ptr = std::make_unique<int>(420);
  EXPECT_EQ(repr(int_ptr.get()), "new int{420}");
  EXPECT_EQ(repr((testing::Test*)0x0), "(testing::Test*)0x0");
  EXPECT_EQ(repr((void*)0x123ff), "(void*)0x123ff");
}

//NOLINTEND
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif


TEST(pointer, string_literal) {
  EXPECT_EQ(repr("foo"), R"("foo")");
}
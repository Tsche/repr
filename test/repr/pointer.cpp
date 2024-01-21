#include <gtest/gtest.h>
#include <repr>
#include <librepr/macro/warning.h>

TEST(pointer, nullptr) {
  EXPECT_EQ(repr(nullptr), "nullptr");
}

struct Test {};

LIBREPR_WARNING_PUSH
LIBREPR_WARNING_DISABLE(GCC, "-Wold-style-cast")
LIBREPR_WARNING_DISABLE(CLANG, "-Wold-style-cast")
//NOLINTBEGIN

TEST(pointer, pointer) {
  auto int_ptr = std::make_unique<int>(420);
  EXPECT_EQ(repr(int_ptr.get()), "new int{420}");
  EXPECT_EQ(repr((testing::Test*)0x0), "(testing::Test*)0x0");
  EXPECT_EQ(repr((void*)0x123ff), "(void*)0x123ff");
}

//NOLINTEND
LIBREPR_WARNING_POP

TEST(pointer, string_literal) {
  EXPECT_EQ(repr("foo"), R"("foo")");
}
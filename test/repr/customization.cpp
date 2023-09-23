#include <gtest/gtest.h>
#include <repr>
#include <string>

struct CustomType {
  std::string repr() const {
    return "custom_repr";
  }
};

TEST(customization, repr_member) {
  EXPECT_EQ(repr(CustomType{}), "custom_repr");
}

struct CustomType2 {};

template <>
std::string librepr::repr(CustomType2 const&){
  return "custom_repr2";
}

TEST(customization, specialization) {
  EXPECT_EQ(repr(CustomType2{}), "custom_repr2");
}
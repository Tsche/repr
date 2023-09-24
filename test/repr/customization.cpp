#include <gtest/gtest.h>
#include <repr>
#include <librepr/object_info.h>
#include <string>

struct CustomType {
  std::string repr() const { return "custom_repr"; }
};

TEST(customization, repr_member) {
  EXPECT_EQ(repr(CustomType{}), "custom_repr");
}

struct CustomType2 {};

namespace librepr {
template <>
ObjectInfo<CustomType2, Literal> repr(CustomType2 const& obj) {
  return {obj, "custom_repr2"};
}
}  // namespace librepr

TEST(customization, specialization) {
  EXPECT_EQ(repr(CustomType2{}), "custom_repr2");
}
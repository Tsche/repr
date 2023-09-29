#include <gtest/gtest.h>
#include <repr>
#include <librepr/object_info.h>
#include <string>

struct CustomType {
  std::string text{};

  [[nodiscard]] std::string repr() const { return text; }
};

TEST(customization, repr_member) {
  EXPECT_EQ(repr(CustomType{"custom_repr"}), "custom_repr");
}

struct CustomType2 {
  explicit CustomType2(){}
};

namespace librepr {
template <>
std::string repr(CustomType2 const&) {
  return "custom_repr2";
}
}  // namespace librepr

TEST(customization, specialization) {
  EXPECT_EQ(repr(CustomType2{}), "custom_repr2");
}
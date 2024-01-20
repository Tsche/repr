#include <gtest/gtest.h>
#include <repr>
#include <string>

struct CustomType {
  std::string text{};

  [[nodiscard]] std::string repr() const { return text; }
};

TEST(customization, repr_member) {
  EXPECT_EQ(repr(CustomType{"custom_repr"}), "CustomType{custom_repr}");
}

struct CustomLiteral {
  std::string text{};

  [[nodiscard]] std::string repr() const { return text; }
};

template <>
struct librepr::is_literal<CustomLiteral> {
  constexpr static bool value = true;
};

TEST(customization, literal_repr_member) {
  EXPECT_EQ(repr(CustomLiteral{"custom_repr"}), "custom_repr");
}

struct CustomType2 {
  explicit CustomType2() {}
};

template <>
std::string librepr::repr(CustomType2 const&) {
  return "{custom_repr2}";
}

TEST(customization, specialization) {
  EXPECT_EQ(repr(CustomType2{}), "CustomType2{custom_repr2}");
}

struct CustomLiteral2 {
  explicit CustomLiteral2() {}
};

template <>
std::string librepr::repr(CustomLiteral2 const&) {
  return "custom_repr2";
}

template <>
struct librepr::is_literal<CustomLiteral2> {
  constexpr static bool value = true;
};

TEST(customization, literal_specialization) {
  EXPECT_EQ(repr(CustomLiteral2{}), "custom_repr2");
}
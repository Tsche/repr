#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <librepr/visitors/repr.h>

namespace librepr::Testing {

TEST(ReprVisitorTest, VisitInt) {
  auto visitor = ReprVisitor(Options{});
  visitor(42);
  ASSERT_EQ(visitor.result, "42");
}

TEST(ReprVisitorTest, VisitStringLiteral) {
  auto visitor = ReprVisitor(Options{});
  visitor("Hello, World!");
  ASSERT_EQ(visitor.result, "\"Hello, World!\"");
}

TEST(ReprVisitorTest, VisitPointer) {
  int testInt    = 42;  // NOLINT
  int* testValue = &testInt;
  auto visitor   = ReprVisitor(Options{});

  visitor(testValue);
  ASSERT_EQ(visitor.result, "new int{42}");
}

TEST(ReprVisitorTest, VisitNullPointer) {
  int* testValue = nullptr;
  auto visitor   = ReprVisitor(Options{});

  visitor(testValue);
  ASSERT_EQ(visitor.result, "(int*)0x0");
}

TEST(ReprVisitorTest, VisitEnum) {
  enum class TestEnum { Value1, Value2 };
  TestEnum testValue = TestEnum::Value1;
  auto visitor       = ReprVisitor(Options{});

  visitor(testValue);

  ASSERT_THAT(visitor.result, testing::HasSubstr("TestEnum::Value1"));
}

TEST(ReprVisitorTest, VisitCustomTypeWithReprMember) {
  struct CustomType {
    [[nodiscard]] std::string repr() const { return "CustomTypeRepr"; }
  };
  CustomType testValue;
  auto visitor = ReprVisitor(Options{});

  visitor(testValue);

  ASSERT_THAT(visitor.result, testing::HasSubstr("CustomType{CustomTypeRepr}"));
}

TEST(ReprVisitorTest, VisitCustomTypeWithoutReprMember) {
  struct CustomType {};
  CustomType testValue;
  auto visitor = ReprVisitor(Options{});

  visitor(testValue);
  ASSERT_THAT(visitor.result, testing::HasSubstr("CustomType"));
}

TEST(ReprVisitorTest, Nesting) {
  auto visitor = ReprVisitor(Options{});
  visitor.increase_nesting();
  ASSERT_EQ(visitor.result, "{");
  visitor.decrease_nesting();
  ASSERT_EQ(visitor.result, "{}");
}

TEST(ReprVisitorTest, Types) {
  auto visitor = ReprVisitor(Options{});
  visitor.operator()<std::type_identity<int>>();
  ASSERT_EQ(visitor.result, "std::type_identity<int>");
  visitor.operator()<std::vector<int>>();
  ASSERT_EQ(visitor.result, "std::type_identity<int>, std::vector<int>");
}

}  // namespace librepr::Testing
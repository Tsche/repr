#include <gtest/gtest.h>
#include <repr>
#include <librepr/reflection/reflect.h>

struct Unreprable{
  explicit Unreprable() = default;
};

TEST(layout, fallback) {
  EXPECT_EQ(librepr::layout_of<Unreprable>(), "Unreprable");
}
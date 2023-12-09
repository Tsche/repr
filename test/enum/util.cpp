#include <concepts>
#include <type_traits>

#include <gtest/gtest.h>
#include <librepr/enum/util.h>
#include "test_enums.h"

using test_enum = test_enums::scoped::Unsigned;
using target_type = std::underlying_type_t<test_enum>;

// NOLINTBEGIN(clang-diagnostic-old-style-cast)
namespace librepr::ctei::test {

TEST(enum_util, to_underlying) {
  EXPECT_EQ((to_underlying<test_enum, EnumKind::Linear>(0)), (target_type)0);
  EXPECT_EQ((to_underlying<test_enum, EnumKind::Linear>(1)), (target_type)1);
  EXPECT_EQ((to_underlying<test_enum, EnumKind::Linear>(5)), (target_type)5);

  EXPECT_EQ((to_underlying<test_enum, EnumKind::Flags>(1)), (target_type)1);
  EXPECT_EQ((to_underlying<test_enum, EnumKind::Flags>(2)), (target_type)2);
  // 5th flag is 1<<4, since the 0th is 0 and 1<<0 == 1
  EXPECT_EQ((to_underlying<test_enum, EnumKind::Flags>(5)), (target_type)(1U << 4U));
}

TEST(enum_util, to_enum) {
  EXPECT_EQ((to_enum<test_enum, 1>()), test_enum::U_A);
  EXPECT_EQ((to_enum<test_enum, 16>()), (test_enum)16);
  EXPECT_EQ((to_enum<test_enum, test_enum::U_A>()), test_enum::U_A);
}

TEST(enum_util, get_enum_name) {
  EXPECT_EQ((enum_name<test_enum, 1>), "test_enums::scoped::Unsigned::U_A");
  EXPECT_EQ((enum_name<test_enum, 0>), "");
}

TEST(enum_util, dump_quick) {
  EXPECT_EQ(dump_quick<test_enum::U_A>(), "test_enums::scoped::Unsigned::U_A");
  EXPECT_EQ(dump_quick<(test_enum)0>(), "(test_enums::scoped::Unsigned)0");
}

TEST(enum_util, dump_list) {
  EXPECT_EQ(dump_list<test_enum::U_A>(), "test_enums::scoped::Unsigned::U_A");
  EXPECT_EQ(dump_list<(test_enum)0>(), "(test_enums::scoped::Unsigned)0");
  EXPECT_EQ((dump_list<(test_enum)0, test_enum::U_A>()), 
             "(test_enums::scoped::Unsigned)0, test_enums::scoped::Unsigned::U_A");
}

TEST(enum_util, is_enum_value) {
  EXPECT_TRUE((is_enum_value<test_enum, 1>()));
  EXPECT_TRUE((is_enum_value<test_enum, test_enum::U_A>()));
  EXPECT_FALSE((is_enum_value<test_enum, 16>()));
}
}
// NOLINTEND(clang-diagnostic-old-style-cast)
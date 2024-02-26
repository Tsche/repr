#include <gtest/gtest.h>
#include <librepr/enum/search.h>
#include "test_enums.h"

namespace librepr::ctei::test {

TEST(enum, flag_kind) {
  EXPECT_EQ(dump_enum<test_enums::unscoped::Flags>::kind, EnumKind::Flags);
  EXPECT_EQ(dump_enum<test_enums::scoped::Flags>::kind, EnumKind::Flags);
  EXPECT_EQ(dump_enum<test_enums::unscoped::FlagsNoUnderlying>::kind, EnumKind::Flags);

  // https://eel.is/c++draft/dcl.enum#5
  // For a scoped enumeration type, the underlying type is int if it is not explicitly specified.
  // TODO Flag detection is disabled for signed types, perhaps special case this if all values are positive?
  EXPECT_EQ(dump_enum<test_enums::scoped::FlagsNoUnderlying>::kind, EnumKind::Linear);
}

TEST(enum, empty_kind) {
  EXPECT_EQ(dump_enum<test_enums::unscoped::Empty>::kind, EnumKind::Empty);
  EXPECT_EQ(dump_enum<test_enums::scoped::Empty>::kind, EnumKind::Empty);
}

TEST(enum, linear_kind) {
  EXPECT_EQ(dump_enum<test_enums::unscoped::NoUnderlying>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::scoped::NoUnderlying>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::unscoped::Signed>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::scoped::Signed>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::unscoped::Unsigned>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::scoped::Unsigned>::kind, EnumKind::Linear);
}

TEST(enum, flag_names) {
  EXPECT_EQ(dump_enum<test_enums::unscoped::Flags>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::scoped::Flags>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::unscoped::FlagsNoUnderlying>::get_names().size(), 5);
  
  // enum class with no underlying type will not be considered flag-like
  // hence the last 2 enumerators will not be found
  EXPECT_EQ(dump_enum<test_enums::scoped::FlagsNoUnderlying>::get_names().size(), 3);
}

TEST(enum, empty_names) {
  EXPECT_EQ(dump_enum<test_enums::unscoped::Empty>::get_names().size(), 0);
  EXPECT_EQ(dump_enum<test_enums::scoped::Empty>::get_names().size(), 0);
}

TEST(enum, linear_names) {
  EXPECT_EQ(dump_enum<test_enums::unscoped::NoUnderlying>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::unscoped::Signed>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::unscoped::Unsigned>::get_names().size(), 5);
  
  EXPECT_EQ(dump_enum<test_enums::scoped::NoUnderlying>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::scoped::Signed>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::scoped::Unsigned>::get_names().size(), 5);
}
}
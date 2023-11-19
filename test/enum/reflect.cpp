#include <gtest/gtest.h>
#include <librepr/enum/search.h>

namespace test_enums {

enum Empty : unsigned {};
enum NoUnderlying { NU_A, NU_B, NU_C, NU_D, NU_E };
enum Signed : int { S_A = -2, S_B = -1, S_C = 0, S_D, S_E };
enum Unsigned : unsigned { U_A = 1, U_B, U_C, U_D = 5, U_E };
enum Flags : unsigned { F_A = 0, F_B = 1U << 0U, F_C = 1U << 1U, F_D = 1U << 10U, F_E = 1U << 11U };
enum FlagsNoUnderlying { FNU_A = 0, FNU_B = 1U << 0U, FNU_C = 1U << 1U, FNU_D = 1U << 10U, FNU_E = 1U << 11U };

enum class EmptyScoped : unsigned {};
enum class NoUnderlyingScoped { NU_A, NU_B, NU_C, NU_D, NU_E };
enum class SignedScoped : int { S_A = -2, S_B = -1, S_C = 0, S_D, S_E };
enum class UnsignedScoped : unsigned { U_A = 1, U_B, U_C, U_D = 5, U_E };
enum class FlagsScoped : unsigned { F_A = 0, F_B = 1U << 0U, F_C = 1U << 1U, F_D = 1U << 10U, F_E = 1U << 11U };
enum class FlagsNoUnderlyingScoped {
  FNU_A = 0,
  FNU_B = 1U << 0U,
  FNU_C = 1U << 1U,
  FNU_D = 1U << 10U,
  FNU_E = 1U << 11U
};

}  // namespace test_enums

TEST(enum, flag_kind) {
  using namespace librepr::ctei;

  EXPECT_EQ(dump_enum<test_enums::Flags>::kind, EnumKind::Flags);
  EXPECT_EQ(dump_enum<test_enums::FlagsScoped>::kind, EnumKind::Flags);
  EXPECT_EQ(dump_enum<test_enums::FlagsNoUnderlying>::kind, EnumKind::Flags);

  // https://eel.is/c++draft/dcl.enum#5
  // For a scoped enumeration type, the underlying type is int if it is not explicitly specified.
  // TODO Flag detection is disabled for signed types, perhaps special case this if all values are positive?
  EXPECT_EQ(dump_enum<test_enums::FlagsNoUnderlyingScoped>::kind, EnumKind::Linear);
}

TEST(enum, empty_kind) {
  using namespace librepr::ctei;

  EXPECT_EQ(dump_enum<test_enums::Empty>::kind, EnumKind::Empty);
  EXPECT_EQ(dump_enum<test_enums::EmptyScoped>::kind, EnumKind::Empty);
}

TEST(enum, linear_kind) {
  using namespace librepr::ctei;

  EXPECT_EQ(dump_enum<test_enums::NoUnderlying>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::NoUnderlyingScoped>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::Signed>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::SignedScoped>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::Unsigned>::kind, EnumKind::Linear);
  EXPECT_EQ(dump_enum<test_enums::UnsignedScoped>::kind, EnumKind::Linear);
}

TEST(enum, flag_names) {
  using namespace librepr::ctei;

  EXPECT_EQ(dump_enum<test_enums::Flags>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::FlagsScoped>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::FlagsNoUnderlying>::get_names().size(), 5);

  // https://eel.is/c++draft/dcl.enum#5
  // For a scoped enumeration type, the underlying type is int if it is not explicitly specified.
  // TODO Flag detection is disabled for signed types, perhaps special case this if all values are positive?
  EXPECT_EQ(dump_enum<test_enums::FlagsNoUnderlyingScoped>::kind, EnumKind::Linear);
}

TEST(enum, empty_names) {
  using namespace librepr::ctei;

  EXPECT_EQ(dump_enum<test_enums::Empty>::get_names().size(), 0);
  EXPECT_EQ(dump_enum<test_enums::EmptyScoped>::get_names().size(), 0);
}

TEST(enum, linear_names) {
  using namespace librepr::ctei;

  EXPECT_EQ(dump_enum<test_enums::NoUnderlying>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::Signed>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::Unsigned>::get_names().size(), 5);
  
  EXPECT_EQ(dump_enum<test_enums::NoUnderlyingScoped>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::SignedScoped>::get_names().size(), 5);
  EXPECT_EQ(dump_enum<test_enums::UnsignedScoped>::get_names().size(), 5);
}
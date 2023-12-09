#pragma once
#include <librepr/customization.h>

#define REPR_TEST_GEN_ENUM(name, underlying, ...) \
  namespace scoped {                              \
  enum class name underlying { __VA_ARGS__ };     \
  }                                               \
  namespace unscoped {                            \
  enum name underlying { __VA_ARGS__ };           \
  }

namespace test_enums {
REPR_TEST_GEN_ENUM(Empty, : unsigned, )
REPR_TEST_GEN_ENUM(Zero, : unsigned, Z_A = 0)
REPR_TEST_GEN_ENUM(NoUnderlying, , NU_A, NU_B, NU_C, NU_D, NU_E)

REPR_TEST_GEN_ENUM(Flags, : unsigned, F_A = 0, F_B = 1U << 0U, F_C = 1U << 1U, F_D = 1U << 10U, F_E = 1U << 11U)
REPR_TEST_GEN_ENUM(FlagsNoUnderlying,
                   ,
                   FNU_A = 0,
                   FNU_B = 1U << 0U,
                   FNU_C = 1U << 1U,
                   FNU_D = 1U << 10U,
                   FNU_E = 1U << 11U)

REPR_TEST_GEN_ENUM(Boolean, : bool, )

// clang-format off
REPR_TEST_GEN_ENUM(UnsignedChar,     : unsigned char,      UC_A  = 1,  UC_B,  UC_C, UC_D  = 5, UC_E)
REPR_TEST_GEN_ENUM(Unsigned,         : unsigned,           U_A   = 1,   U_B,   U_C, U_D   = 5, U_E)
REPR_TEST_GEN_ENUM(UnsignedShort,    : unsigned short,     US_A  = 1,  US_B,  US_C, US_D  = 5, US_E)
REPR_TEST_GEN_ENUM(UnsignedLong,     : unsigned long,      UL_A  = 1,  UL_B,  UL_C, UL_D  = 5, UL_E)
REPR_TEST_GEN_ENUM(UnsignedLongLong, : unsigned long long, ULL_A = 1, ULL_B, ULL_C, ULL_D = 5, ULL_E)

REPR_TEST_GEN_ENUM(SignedChar,     : signed char,      SC_A  = -2, SC_B  = -1, SC_C  = 0, SC_D,  SC_E)
REPR_TEST_GEN_ENUM(Signed,         : signed,           S_A   = -2, S_B   = -1, S_C   = 0, S_D,   S_E)
REPR_TEST_GEN_ENUM(SignedShort,    : signed short,     SS_A  = -2, SS_B  = -1, SS_C  = 0, SS_D,  SS_E)
REPR_TEST_GEN_ENUM(SignedLong,     : signed long,      SL_A  = -2, SL_B  = -1, SL_C  = 0, SL_D,  SL_E)
REPR_TEST_GEN_ENUM(SignedLongLong, : signed long long, SLL_A = -2, SLL_B = -1, SLL_C = 0, SLL_D, SLL_E)
// clang-format on

REPR_TEST_GEN_ENUM(Custom, : signed long long, CUSTOM_A = -2, CUSTOM_B = -1, CUSTOM_C = 0, CUSTOM_D, CUSTOM_E)
REPR_TEST_GEN_ENUM(CustomFlag, : unsigned long long, CF_A = 1ULL << 8U, CF_B = 1ULL << 9U, CF_C = 1ULL << 12U)
}  // namespace test_enums

template <>
struct librepr::EnumSettings<test_enums::scoped::Custom>{ 
  struct search_range {
    static constexpr auto min = -10;
    static constexpr auto max = 10;
  };
};

template <>
struct librepr::EnumSettings<test_enums::unscoped::Custom>{ 
  struct search_range {
    static constexpr auto min = -10;
    static constexpr auto max = 10;
  };
};

template <>
struct librepr::EnumSettings<test_enums::scoped::CustomFlag>{ 
  constexpr static bool is_flag = true;
};

template <>
struct librepr::EnumSettings<test_enums::unscoped::CustomFlag>{ 
  constexpr static bool is_flag = true;
};
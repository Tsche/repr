#pragma once
#include <librepr/customization.h>
#include <cstdint>
#include "constants.h"
#include "token/generic.h"
#include "token/numeral.h"
#include "token/name.h"
#include "keywords.h"
namespace librepr::parsing {

namespace CharacterFlags {
enum Flag : unsigned { is_plain = 0, is_raw = 1U << 0U, is_wide = 1U << 1U };
};

namespace StringFlags {
enum Flag : unsigned {
  is_plain = 0,
  is_raw   = 1U << 0U,
  is_wide  = 1U << 1U,
};
};  // namespace StringFlags

}  // namespace librepr::parsing

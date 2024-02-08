#pragma once
#include <librepr/customization.h>

namespace librepr::parsing {
namespace TokenCategory {
enum Category : unsigned { error, generic, character, string, numeric };
}

namespace TokenKind {
  enum Kind : unsigned {
    invalid,
    eof,
    identifier,
    numeric_literal,
    string_literal,
    char_literal,

    l_square,             // [
    r_square,             // ]
    l_paren,              // (
    r_paren,              // )
    l_brace,              // {
    r_brace,              // }
    period,               // .
    amp,                  // &
    ampamp,               // &&
    ampequal,             // &=
    star,                 // *
    starequal,            // *=
    plus,                 // +
    plusplus,             // ++
    plusequal,            // +=
    minus,                // -
    arrow,                // ->
    minusminus,           // --
    minusequal,           // -=
    tilde,                // ~
    exclaim,              // !
    exclaimequal,         // !=
    slash,                // /
    slashequal,           // /=
    percent,              // %
    percentequal,         // %=
    less,                 // <
    lessless,             // <<
    lessequal,            // <=
    lesslessequal,        // <<=
    spaceship,            // <=>
    greater,              // >
    greatergreater,       // >>
    greaterequal,         // >=
    greatergreaterequal,  // >>=
    caret,                // ^
    caretequal,           // ^=
    pipe,                 // |
    pipepipe,             // ||
    pipeequal,            // |=
    question,             // ?
    colon,                // :
    coloncolon,           // ::
    semi,                 // ;
    equal,                // =
    equalequal,           // ==
    comma,                // ,
    periodstar,           // .*
    arrowstar,            // ->*
  };
};

namespace CharacterFlags {
  enum Flag : unsigned { is_plain = 0, is_raw = 1U << 0U, is_wide = 1U << 1U };
};

template <typename T, auto Tag>
struct TaggedEnum {
  constexpr static auto tag = Tag;
};

namespace StringFlags {
  enum Flag : unsigned {
    is_plain = 0,
    is_raw   = 1U << 0U,
    is_wide  = 1U << 1U,
  };

  inline auto librepr_settings(Flag){
    return TaggedEnum<Flag, TokenCategory::string>{};
  }
};

struct Numeral {
  constexpr static auto tag = TokenCategory::numeric;
  enum Flags : unsigned char {
    sign        = 1U << 0U,
    is_floating = 1U << 1U,
    has_prefix  = 1U << 2U,
    has_suffix  = 1U << 3U
  } flags;

  enum Base : unsigned char { binary = 1U << 0U, octal = 1U << 1U, decimal = 1U << 2U, hex = 1U << 3U } base;

  enum Floats : unsigned char {
    float_      = 1U << 0U,
    double_     = 1U << 1U,
    long_double = 1U << 2U,
    float16     = 1U << 3U,
    float32     = 1U << 4U,
    float64     = 1U << 5U,
    float128    = 1U << 6U,
    bfloat16    = 1U << 7U
  };

  enum Integers : unsigned char {
    int_               = 1U << 0U,
    unsigned_          = 1U << 1U,
    long_              = 1U << 2U,
    unsigned_long      = 1U << 3U,
    long_long          = 1U << 4U,
    unsigned_long_long = 1U << 5U,
    ssize_t            = 1U << 6U,
    size_t             = 1U << 7U
  };

  union {
    Floats floating;
    Integers integer;
  } kind;

  [[nodiscard]] constexpr std::size_t bit_width() const {
    if (is_float()) {
      switch (kind.floating) {
        case float_:
          return sizeof(float) * 8;
        case double_:
          return sizeof(double) * 8;
        case long_double:
          return sizeof(long double) * 8;
        case float16:
        case bfloat16:
          return 16;
        case float32:
          return 32;
        case float64:
          return 64;
        case float128:
          return 128;
      }
    } else {
      switch (kind.integer) {
        case int_:
        case unsigned_:
          return sizeof(int) * 8;
        case long_:
        case unsigned_long:
          return sizeof(long) * 8;
        case long_long:
        case unsigned_long_long:
          return sizeof(long long) * 8;
        case size_t:
        case ssize_t:
          return sizeof(std::size_t) * 8;
      }
    }
    return 0;
  }

  [[nodiscard]] constexpr bool is_float() const { return (flags & is_floating) != 0; }

  [[nodiscard]] constexpr bool is_signed() const {
    if (is_float()) {
      return true;
    }
    switch (kind.integer) {
      case int_:
      case long_:
      case long_long:
      case ssize_t:
        return true;
      case unsigned_:
      case unsigned_long:
      case unsigned_long_long:
      case size_t:
        return false;
    }

    // TODO use unreachable/throw here?
    return false;
  }

  [[nodiscard]] constexpr bool is_negative() const { return (flags & sign) != 0; }

  [[nodiscard]] constexpr bool is(Base base_) const { return base == base_; }

  [[nodiscard]] constexpr bool is(Integers type_) const { return !is_float() && kind.integer == type_; }

  [[nodiscard]] constexpr bool is(Floats type_) const { return is_float() && kind.floating == type_; }

  [[nodiscard]] constexpr bool in(Integers types) const { return !is_float() && (kind.integer & types) != 0; }

  [[nodiscard]] constexpr bool in(Floats types) const { return is_float() && (kind.floating & types) != 0; }

  constexpr void set(Floats type_) {
    flags         = static_cast<Flags>(flags | is_floating);
    kind.floating = type_;
  }
  constexpr void set(Integers type_) {
    flags        = static_cast<Flags>(flags & static_cast<Flags>(~is_floating));
    kind.integer = type_;
  }

  constexpr void set(Flags flag) {
    flags = (Flags)(flag | flags);
  }
};
static_assert(sizeof(Numeral) <= 4);
}  // namespace librepr::parsing


template<>
struct librepr::Settings<librepr::parsing::TokenKind::Kind> {
  static constexpr auto tag = librepr::parsing::TokenCategory::generic;
};

template<>
struct librepr::Settings<librepr::parsing::CharacterFlags::Flag> {
static constexpr auto tag = librepr::parsing::TokenCategory::character;
};

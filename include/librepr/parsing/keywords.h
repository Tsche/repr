/* C++ code produced by gperf version 3.1 */
/* Command-line: /usr/bin/gperf -j 1 -m 100 -D keywords.gperf  */
/* Computed positions: -k'1,3,5,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) && ('%' == 37) && ('&' == 38) && ('\'' == 39) &&       \
      ('(' == 40) && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) && ('-' == 45) && ('.' == 46) &&        \
      ('/' == 47) && ('0' == 48) && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) && ('5' == 53) &&        \
      ('6' == 54) && ('7' == 55) && ('8' == 56) && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) &&        \
      ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) && ('B' == 66) && ('C' == 67) && ('D' == 68) &&        \
      ('E' == 69) && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) && ('J' == 74) && ('K' == 75) &&        \
      ('L' == 76) && ('M' == 77) && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) && ('R' == 82) &&        \
      ('S' == 83) && ('T' == 84) && ('U' == 85) && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) &&        \
      ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) && ('^' == 94) && ('_' == 95) && ('a' == 97) &&       \
      ('b' == 98) && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) && ('g' == 103) && ('h' == 104) &&   \
      ('i' == 105) && ('j' == 106) && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) && ('o' == 111) && \
      ('p' == 112) && ('q' == 113) && ('r' == 114) && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) && \
      ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) && ('{' == 123) && ('|' == 124) && ('}' == 125) && \
      ('~' == 126))
/* The character set is not based on ISO-646.  */
#  error \
      "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "keywords.gperf"

#pragma once
#include "keyword.h"

namespace librepr::parsing::detail {
// NOLINT
enum {
  KW_TOTAL_KEYWORDS  = 138,
  KW_MIN_WORD_LENGTH = 2,
  KW_MAX_WORD_LENGTH = 22,
  KW_MIN_HASH_VALUE  = 7,
  KW_MAX_HASH_VALUE  = 228
};

/* maximum key range = 222, duplicates = 0 */

class KeywordHash {
private:
  static constexpr unsigned int hash(std::string_view str);

public:
  static constexpr const KeywordEntry* find(std::string_view str);
};

static constexpr unsigned char keywords_asso_values[] = {
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 15,  41,  6,   54,  229, 5,   229, 75,  229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 2,   229, 17,  84,  3,   58,  0,   6,   45,  29,  4,   229, 8,   47,  29,
    11,  85,  67,  43,  16,  23,  0,   94,  35,  7,   77,  4,   3,   229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229};

constexpr unsigned int KeywordHash::hash(std::string_view str) {
  unsigned int hval = str.length();

  switch (hval) {
    default: hval += keywords_asso_values[static_cast<unsigned char>(str[4])];
    /*FALLTHROUGH*/
    case 4:
    case 3: hval += keywords_asso_values[static_cast<unsigned char>(str[2])];
    /*FALLTHROUGH*/
    case 2:
    case 1: hval += keywords_asso_values[static_cast<unsigned char>(str[0])]; break;
  }
  return hval + keywords_asso_values[static_cast<unsigned char>(str[str.length() - 1])];
}

static constexpr const KeywordEntry keywords[] = {
#line 142 "keywords.gperf"
    {"int", Name{Type::Int}},
#line 70 "keywords.gperf"
    {"__event", Name{WindowsKeyword::Event}},
#line 87 "keywords.gperf"
    {"try", Name{Control::Try}},
#line 80 "keywords.gperf"
    {"if", Name{Control::If}},
#line 98 "keywords.gperf"
    {"__except", Name{WindowsSEH::Except}},
#line 153 "keywords.gperf"
    {"not", Name{Operator::Not}},
#line 97 "keywords.gperf"
    {"__try", Name{WindowsSEH::Try}},
#line 84 "keywords.gperf"
    {"while", Name{Control::While}},
#line 73 "keywords.gperf"
    {"__interface", Name{WindowsKeyword::Interface}},
#line 125 "keywords.gperf"
    {"__int16", Name{Type::Int16}},
#line 28 "keywords.gperf"
    {"const", Name{Qualifier::Const}},
#line 94 "keywords.gperf"
    {"co_await", Name{Control::CoAwait}},
#line 44 "keywords.gperf"
    {"concept", Name{Keyword::Concept}},
#line 122 "keywords.gperf"
    {"noexcept", Name{Magic::Noexcept}},
#line 35 "keywords.gperf"
    {"constinit", Name{Specifier::Constinit}},
#line 101 "keywords.gperf"
    {"const_cast", Name{Cast::Const}},
#line 96 "keywords.gperf"
    {"co_return", Name{Control::CoReturn}},
#line 83 "keywords.gperf"
    {"continue", Name{Control::Continue}},
#line 81 "keywords.gperf"
    {"else", Name{Control::Else}},
#line 58 "keywords.gperf"
    {"new", Name{Keyword::New}},
#line 77 "keywords.gperf"
    {"__raise", Name{WindowsKeyword::Raise}},
#line 90 "keywords.gperf"
    {"case", Name{Control::Case}},
#line 52 "keywords.gperf"
    {"this", Name{Keyword::This}},
#line 100 "keywords.gperf"
    {"__finally", Name{WindowsSEH::Finally}},
#line 47 "keywords.gperf"
    {"extern", Name{Keyword::Extern}},
#line 139 "keywords.gperf"
    {"char32_t", Name{Type::Char32}},
#line 86 "keywords.gperf"
    {"throw", Name{Control::Throw}},
#line 103 "keywords.gperf"
    {"reinterpret_cast", Name{Cast::Reinterpret}},
#line 72 "keywords.gperf"
    {"__forceinline", Name{WindowsKeyword::ForceInline}},
#line 108 "keywords.gperf"
    {"__alignof", Name{WindowsMagic::Alignof}},
#line 34 "keywords.gperf"
    {"constexpr", Name{Specifier::Constexpr}},
#line 136 "keywords.gperf"
    {"char", Name{Type::Char}},
#line 82 "keywords.gperf"
    {"for", Name{Control::For}},
#line 110 "keywords.gperf"
    {"__if_exists", Name{WindowsMagic::IfExists}},
#line 138 "keywords.gperf"
    {"char16_t", Name{Type::Char16}},
#line 120 "keywords.gperf"
    {"alignof", Name{Magic::Alignof}},
#line 109 "keywords.gperf"
    {"__if_not_exists", Name{WindowsMagic::IfNotExists}},
#line 68 "keywords.gperf"
    {"__sptr", Name{WindowsKeyword::Sptr}},
#line 39 "keywords.gperf"
    {"struct", Name{ClassKey::Struct}},
#line 79 "keywords.gperf"
    {"return", Name{Control::Return}},
#line 107 "keywords.gperf"
    {"__assume", Name{WindowsMagic::Assume}},
#line 61 "keywords.gperf"
    {"__restrict", Name{WindowsKeyword::Restrict}},
#line 51 "keywords.gperf"
    {"static", Name{Keyword::Static}},
#line 126 "keywords.gperf"
    {"__int32", Name{Type::Int32}},
#line 104 "keywords.gperf"
    {"static_cast", Name{Cast::Static}},
#line 74 "keywords.gperf"
    {"__single_inheritance", Name{WindowsKeyword::SingleInheritance}},
#line 117 "keywords.gperf"
    {"static_assert", Name{Magic::StaticAssert}},
#line 26 "keywords.gperf"
    {"false", Name{Literal::False}},
#line 60 "keywords.gperf"
    {"__cdecl", Name{WindowsKeyword::Cdecl}},
#line 154 "keywords.gperf"
    {"not_eq", Name{Operator::NotEq}},
#line 71 "keywords.gperf"
    {"__inline", Name{WindowsKeyword::Inline}},
#line 119 "keywords.gperf"
    {"alignas", Name{Magic::Alignas}},
#line 63 "keywords.gperf"
    {"__thiscall", Name{WindowsKeyword::ThisCall}},
#line 89 "keywords.gperf"
    {"switch", Name{Control::Switch}},
#line 88 "keywords.gperf"
    {"catch", Name{Control::Catch}},
#line 127 "keywords.gperf"
    {"__int64", Name{Type::Int64}},
#line 36 "keywords.gperf"
    {"inline", Name{Specifier::Inline}},
#line 123 "keywords.gperf"
    {"decltype", Name{Magic::Decltype}},
#line 33 "keywords.gperf"
    {"consteval", Name{Specifier::Consteval}},
#line 38 "keywords.gperf"
    {"class", Name{ClassKey::Class}},
#line 45 "keywords.gperf"
    {"namespace", Name{Keyword::Namespace}},
#line 99 "keywords.gperf"
    {"__leave", Name{WindowsSEH::Leave}},
#line 76 "keywords.gperf"
    {"__virtual_inheritance", Name{WindowsKeyword::VirtualInheritance}},
#line 95 "keywords.gperf"
    {"co_yield", Name{Control::CoYield}},
#line 105 "keywords.gperf"
    {"__declspec", Name{WindowsMagic::Declspec}},
#line 114 "keywords.gperf"
    {"asm", Name{Magic::Asm}},
#line 121 "keywords.gperf"
    {"explicit", Name{Magic::Explicit}},
#line 59 "keywords.gperf"
    {"__asm", Name{WindowsKeyword::Asm}},
#line 131 "keywords.gperf"
    {"__m128i", Name{Type::M128i}},
#line 42 "keywords.gperf"
    {"template", Name{Keyword::Template}},
#line 48 "keywords.gperf"
    {"friend", Name{Keyword::Friend}},
#line 43 "keywords.gperf"
    {"typename", Name{Keyword::Typename}},
#line 124 "keywords.gperf"
    {"__int8", Name{Type::Int8}},
#line 62 "keywords.gperf"
    {"__fastcall", Name{WindowsKeyword::FastCall}},
#line 46 "keywords.gperf"
    {"export", Name{Keyword::Export}},
#line 29 "keywords.gperf"
    {"volatile", Name{Qualifier::Volatile}},
#line 53 "keywords.gperf"
    {"thread_local", Name{Keyword::ThreadLocal}},
#line 161 "keywords.gperf"
    {"import", Name{Contextual::Import}},
#line 115 "keywords.gperf"
    {"requires", Name{Magic::Requires}},
#line 30 "keywords.gperf"
    {"private", Name{AccessSpecifier::Private}},
#line 141 "keywords.gperf"
    {"float", Name{Type::Float}},
#line 25 "keywords.gperf"
    {"true", Name{Literal::True}},
#line 65 "keywords.gperf"
    {"__vectorcall", Name{WindowsKeyword::VectorCall}},
#line 75 "keywords.gperf"
    {"__multiple_inheritance", Name{WindowsKeyword::MultipleInheritance}},
#line 143 "keywords.gperf"
    {"void", Name{Type::Void}},
#line 137 "keywords.gperf"
    {"char8_t", Name{Type::Char8}},
#line 155 "keywords.gperf"
    {"or", Name{Operator::Or}},
#line 92 "keywords.gperf"
    {"break", Name{Control::Break}},
#line 134 "keywords.gperf"
    {"auto", Name{Type::Auto}},
#line 144 "keywords.gperf"
    {"long", Name{Modifier::Long}},
#line 50 "keywords.gperf"
    {"register", Name{Keyword::Register}},
#line 160 "keywords.gperf"
    {"override", Name{Contextual::Override}},
#line 102 "keywords.gperf"
    {"dynamic_cast", Name{Cast::Dynamic}},
#line 57 "keywords.gperf"
    {"delete", Name{Keyword::Delete}},
#line 157 "keywords.gperf"
    {"xor", Name{Operator::Xor}},
#line 145 "keywords.gperf"
    {"short", Name{Modifier::Short}},
#line 113 "keywords.gperf"
    {"__uuidof", Name{WindowsMagic::UuidOf}},
#line 78 "keywords.gperf"
    {"__super", Name{WindowsKeyword::Super}},
#line 159 "keywords.gperf"
    {"final", Name{Contextual::Final}},
#line 69 "keywords.gperf"
    {"__uptr", Name{WindowsKeyword::Uptr}},
#line 37 "keywords.gperf"
    {"mutable", Name{Specifier::Mutable}},
#line 151 "keywords.gperf"
    {"bitor", Name{Operator::BitOr}},
#line 66 "keywords.gperf"
    {"__w64", Name{WindowsKeyword::W64}},
#line 116 "keywords.gperf"
    {"sizeof", Name{Magic::Sizeof}},
#line 149 "keywords.gperf"
    {"and_eq", Name{Operator::AndEq}},
#line 40 "keywords.gperf"
    {"union", Name{ClassKey::Union}},
#line 49 "keywords.gperf"
    {"operator", Name{Keyword::Operator}},
#line 41 "keywords.gperf"
    {"enum", Name{EnumKey::Enum}},
#line 111 "keywords.gperf"
    {"__hook", Name{WindowsMagic::Hook}},
#line 152 "keywords.gperf"
    {"compl", Name{Operator::Compl}},
#line 146 "keywords.gperf"
    {"signed", Name{Modifier::Signed}},
#line 132 "keywords.gperf"
    {"__ptr32", Name{Type::Ptr32}},
#line 93 "keywords.gperf"
    {"goto", Name{Control::Goto}},
#line 118 "keywords.gperf"
    {"typeid", Name{Magic::Typeid}},
#line 148 "keywords.gperf"
    {"and", Name{Operator::And}},
#line 130 "keywords.gperf"
    {"__m128d", Name{Type::M128d}},
#line 54 "keywords.gperf"
    {"typedef", Name{Keyword::Typedef}},
#line 64 "keywords.gperf"
    {"__stdcall", Name{WindowsKeyword::StdCall}},
#line 162 "keywords.gperf"
    {"module", Name{Contextual::Module}},
#line 112 "keywords.gperf"
    {"__unhook", Name{WindowsMagic::Unhook}},
#line 158 "keywords.gperf"
    {"xor_eq", Name{Operator::XorEq}},
#line 128 "keywords.gperf"
    {"__m64", Name{Type::M64}},
#line 85 "keywords.gperf"
    {"do", Name{Control::Do}},
#line 133 "keywords.gperf"
    {"__ptr64", Name{Type::Ptr64}},
#line 27 "keywords.gperf"
    {"nullptr", Name{Literal::Nullptr}},
#line 129 "keywords.gperf"
    {"__m128", Name{Type::M128}},
#line 150 "keywords.gperf"
    {"bitand", Name{Operator::BitAnd}},
#line 32 "keywords.gperf"
    {"public", Name{AccessSpecifier::Public}},
#line 91 "keywords.gperf"
    {"default", Name{Control::Default}},
#line 106 "keywords.gperf"
    {"__based", Name{WindowsMagic::Based}},
#line 156 "keywords.gperf"
    {"or_eq", Name{Operator::OrEq}},
#line 67 "keywords.gperf"
    {"__unaligned", Name{WindowsKeyword::Unaligned}},
#line 55 "keywords.gperf"
    {"using", Name{Keyword::Using}},
#line 56 "keywords.gperf"
    {"virtual", Name{Keyword::Virtual}},
#line 140 "keywords.gperf"
    {"double", Name{Type::Double}},
#line 31 "keywords.gperf"
    {"protected", Name{AccessSpecifier::Protected}},
#line 135 "keywords.gperf"
    {"bool", Name{Type::Bool}},
#line 147 "keywords.gperf"
    {"unsigned", Name{Modifier::Unsigned}}};

constexpr static short keywords_lookup[] = {
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  0,   -1,  1,   -1,  2,   3,   4,   5,  6,   7,   8,   9,   10,  11,  12,  13,
    14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28, 29,  30,  31,  32,  33,  34,  -1,  35,
    36,  37,  38,  39,  40,  41,  -1,  42,  43,  44,  45,  46,  47,  48,  49, 50,  51,  52,  -1,  53,  54,  55,  56,
    57,  58,  59,  60,  61,  62,  63,  64,  -1,  65,  66,  -1,  -1,  67,  68, 69,  70,  71,  72,  73,  74,  75,  -1,
    76,  77,  78,  79,  80,  -1,  81,  82,  83,  84,  85,  86,  -1,  87,  88, 89,  90,  91,  92,  93,  94,  95,  96,
    97,  98,  -1,  99,  -1,  100, 101, 102, 103, 104, 105, 106, 107, -1,  -1, 108, 109, 110, 111, 112, 113, 114, 115,
    116, 117, 118, 119, 120, -1,  121, 122, 123, -1,  124, -1,  -1,  -1,  -1, 125, -1,  -1,  -1,  -1,  -1,  126, -1,
    -1,  -1,  -1,  127, 128, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  129, -1, -1,  -1,  130, -1,  -1,  -1,  131, -1,
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  132, -1,  -1,  -1,  -1,  -1, 133, -1,  -1,  -1,  -1,  -1,  134, -1,
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  135, 136, -1, -1,  -1,  -1,  -1,  -1,  -1,  137};

constexpr const KeywordEntry* KeywordHash::find(std::string_view str) {
  if (str.length() <= KW_MAX_WORD_LENGTH && str.length() >= KW_MIN_WORD_LENGTH) {
    unsigned int key = hash(str);

    if (key <= KW_MAX_HASH_VALUE) {
      int index = keywords_lookup[key];

      if (index >= 0) {
        std::string_view s = keywords[index].key;

        if (str == s)
          return &keywords[index];
      }
    }
  }
  return nullptr;
}
#line 163 "keywords.gperf"

// NOLINTEND
}  // namespace librepr::parsing::detail

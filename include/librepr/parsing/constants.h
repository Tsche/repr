#pragma once
#include <librepr/util/hash.h>

namespace librepr::parsing::constants {
static inline constexpr auto literal = HashSet{"true", "false", "nullptr"};

static inline constexpr auto keyword = HashSet {
  "const", "class", "struct", "enum", "auto", "consteval", "constexpr", "constinit", "template", "typename", "union",
      "concept", "namespace", "export", "extern", "friend", "inline", "mutable", "operator", "private", "protected",
      "public", "register", "static", "this", "thread_local", "typedef", "using", "virtual", "volatile", "default",
      "delete", "new", "export", "import",
      "module"
#if USING(LIBREPR_PLATFORM_WINDOWS)
      "__asm",
      "__cdecl", "__restrict", "__fastcall", "__thiscall", "__stdcall", "__vectorcall", "__w64", "__unaligned", ,
      "__sptr", "__uptr", "__event", "__inline", "__forceinline", "__interface", "__single_inheritance",
      "__multiple_inheritance", "__virtual_inheritance", "__raise", "__super"

#endif
};

static inline constexpr auto control_statement = HashSet {
  "return", "if", "else", "for", "continue", "while", "do", "throw", "try", "catch", "switch", "case", "default",
      "break", "goto", "co_await", "co_yield", "co_return",
#if USING(LIBREPR_PLATFORM_WINDOWS)
      "__try", "__except", "__leave", "__finally"
#endif
};

static inline constexpr auto cast = HashSet{
    "const_cast",
    "dynamic_cast",
    "reinterpret_cast",
    "static_cast",
};
static inline constexpr auto magic = HashSet {
#if USING(LIBREPR_PLATFORM_WINDOWS)
  "__declspec", "__based", "__assume", "__alignof", "__if_not_exists", "__if_exists", "__hook", "__unhook",
      "__uuidof"
#endif
      "asm",
      "requires", "sizeof", "static_assert", "typeid", "alignas", "alignof", "explicit", "noexcept", "decltype",
};

static inline constexpr auto type = HashSet {
#if USING(LIBREPR_PLATFORM_WINDOWS)
  "__int8", "__int16", "__int32", "__int64", "__m64", "__m128", "__m128d", "__m128i", "__ptr32", "__ptr64",
#endif
      "bool", "char", "char8_t", "char16_t", "char32_t", "double", "float", "int", "long", "short", "signed",
      "unsigned", "void",
};

static inline constexpr auto alternative_operator =
    HashSet{"and", "and_eq", "bitand", "bitor", "compl", "not", "not_eq", "or", "or_eq", "xor", "xor_eq"};

static inline constexpr auto contextual_keyword = HashSet{"final", "override", "import", "module"};

static inline constexpr auto preprocessor = HashSet{
    "if",    "elif",    "else", "endif", "ifdef",   "ifndef", "elifdef", "elifndef",      "define",
    "undef", "include", "line", "error", "warning", "pragma", "defined", "__has_include", "__has_cpp_attribute",
};
}  // namespace librepr::parsing::constants
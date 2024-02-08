#pragma once
#include <librepr/util/hash.h>

namespace librepr::parsing::constants {
static inline constexpr auto keywords = HashSet{
    "explicit"_fnv1a, "export"_fnv1a,   "extern"_fnv1a,   "friend"_fnv1a,       "inline"_fnv1a,    "mutable"_fnv1a,
    "noexcept"_fnv1a, "nullptr"_fnv1a,  "operator"_fnv1a, "private"_fnv1a,      "protected"_fnv1a, "public"_fnv1a,
    "register"_fnv1a, "static"_fnv1a,   "this"_fnv1a,     "thread_local"_fnv1a, "typedef"_fnv1a,   "using"_fnv1a,
    "virtual"_fnv1a,  "volatile"_fnv1a, "default"_fnv1a,  "true"_fnv1a,         "false"_fnv1a,

};

static inline constexpr auto type_keywords = HashSet{
    "const"_fnv1a,     "class"_fnv1a,     "struct"_fnv1a,    "enum"_fnv1a,     "auto"_fnv1a,
    "consteval"_fnv1a, "constexpr"_fnv1a, "constinit"_fnv1a, "template"_fnv1a, "typename"_fnv1a,
    "union"_fnv1a,     "concept"_fnv1a,   "namespace"_fnv1a,
};

static inline constexpr auto magic_builtins = HashSet{
    "alignas"_fnv1a,   "alignof"_fnv1a,       "const_cast"_fnv1a,   "co_await"_fnv1a,         "co_yield"_fnv1a,
    "co_return"_fnv1a, "decltype"_fnv1a,      "dynamic_cast"_fnv1a, "reinterpret_cast"_fnv1a, "requires"_fnv1a,
    "sizeof"_fnv1a,    "static_assert"_fnv1a, "static_cast"_fnv1a,  "typeid"_fnv1a,           "delete"_fnv1a,
    "new"_fnv1a,       "asm"_fnv1a,
};

static inline constexpr auto control_statements = HashSet{
    "return"_fnv1a, "if"_fnv1a,   "else"_fnv1a,    "for"_fnv1a,   "continue"_fnv1a,
    "while"_fnv1a,  "do"_fnv1a,   "throw"_fnv1a,   "try"_fnv1a,   "catch"_fnv1a,
    "switch"_fnv1a, "case"_fnv1a, "default"_fnv1a, "break"_fnv1a, "goto"_fnv1a,

};

static inline constexpr auto msvc_keywords =
    HashSet{"__ptr64"_fnv1a, "__cdecl"_fnv1a, "__stdcall"_fnv1a, "__thiscall"_fnv1a, "__fastcall"_fnv1a};

static inline constexpr auto builtin_types = HashSet{
    "bool"_fnv1a, "char"_fnv1a, "char8_t"_fnv1a, "char16_t"_fnv1a, "char32_t"_fnv1a, "double"_fnv1a, "float"_fnv1a,
    "int"_fnv1a,  "long"_fnv1a, "short"_fnv1a,   "signed"_fnv1a,   "unsigned"_fnv1a, "void"_fnv1a,
};

static inline constexpr auto contextual_keywords =
    HashSet{"final"_fnv1a, "override"_fnv1a, "import"_fnv1a, "module"_fnv1a};

static inline constexpr auto preprocessor = HashSet{"if"_fnv1a,      "elif"_fnv1a,          "else"_fnv1a,
                                                    "endif"_fnv1a,   "ifdef"_fnv1a,         "ifndef"_fnv1a,
                                                    "elifdef"_fnv1a, "elifndef"_fnv1a,      "define"_fnv1a,
                                                    "undef"_fnv1a,   "include"_fnv1a,       "line"_fnv1a,
                                                    "error"_fnv1a,   "warning"_fnv1a,       "pragma"_fnv1a,
                                                    "defined"_fnv1a, "__has_include"_fnv1a, "__has_cpp_attribute"_fnv1a,
                                                    "export"_fnv1a,  "import"_fnv1a,        "module"_fnv1a};

static inline constexpr auto alternative_operators =
    HashSet{"and"_fnv1a,    "and_eq"_fnv1a, "bitand"_fnv1a, "bitor"_fnv1a, "compl"_fnv1a, "not"_fnv1a,
            "not_eq"_fnv1a, "or"_fnv1a,     "or_eq"_fnv1a,  "xor"_fnv1a,   "xor_eq"_fnv1a};
}  // namespace librepr::parsing::constants
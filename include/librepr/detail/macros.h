#pragma once

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#define LIBREPR_PRAGMA(...) _Pragma(#__VA_ARGS__)


#if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL
#error "A standard compliant preprocessor is required. Add /Zc:preprocessor to your compiler invocation."
#endif

#define PARENS ()
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN PARENS(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define LIBREPR_MAYBE_COMMA(...) __VA_OPT__(,) __VA_ARGS__
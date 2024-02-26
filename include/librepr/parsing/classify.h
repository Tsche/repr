#pragma once

namespace librepr::parsing {
constexpr inline bool is_alphabetic(char cur) {
    return (cur >= 'A' && cur <= 'Z') || (cur >= 'a' && cur <= 'z');
}

constexpr inline bool is_numeric(char cur) {
    return (cur >= '0' && cur <= '9');
}

constexpr inline bool is_hex(char cur) {
    return is_numeric(cur) || (cur >= 'A' && cur <= 'F') || (cur >= 'a' && cur <= 'f');
}

constexpr inline bool is_ident(char cur) {
    return  is_alphabetic(cur) || (cur == '_');
}

constexpr inline bool is_ident_continue(char cur) {
    return is_ident(cur) || is_numeric(cur);
}

constexpr inline bool is_whitespace(char cur) {
    return (cur == ' ') || (cur == '\t') || (cur == '\n') || (cur == '\r') || (cur == '\f');
}
}
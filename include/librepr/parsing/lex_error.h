#pragma once

#include "token_kind.h"


namespace librepr::parsing::LexError {
  enum Error : unsigned {
        Unknown,
        UnknownCharacter,
        MultipleDecimalPoints,
        ConsecutiveDigitSeparator,
        DigitSeparatorAdjacentToDecimalPoint,
        SingleColon,
        InvalidNumericLiteral,
        UnclosedStringLiteral,
        UnclosedCharacterLiteral,
      };

  [[nodiscard]] constexpr inline char const* get_message(Error value) {
    switch (value) {
            case Unknown:
        return "Unknown error";
            case UnknownCharacter:
        return "Found unrecognized character.";
            case MultipleDecimalPoints:
        return "Multiple decimal points found.";
            case ConsecutiveDigitSeparator:
        return "Consecutive digit separators detected.";
            case DigitSeparatorAdjacentToDecimalPoint:
        return "The decimal point cannot be preceeded or followed by a digit separator.";
            case SingleColon:
        return "Did you mean '::'?";
            case InvalidNumericLiteral:
        return "Invalid numeric literal found.";
            case UnclosedStringLiteral:
        return "String literal was not closed.";
            case UnclosedCharacterLiteral:
        return "Character literal was not closed";
            default:
        break;
    }
    return "unrecognized error";
  }
}  // namespace librepr::parsing::LexError


template<>
struct librepr::Settings<librepr::parsing::LexError::Error> {
static constexpr auto tag = librepr::parsing::TokenCategory::error;
};
#pragma once

namespace librepr::parsing::token::LexError {
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
    case Unknown: return "Unknown error";
    case UnknownCharacter: return "Found unrecognized character.";
    case MultipleDecimalPoints: return "Multiple decimal points found.";
    case ConsecutiveDigitSeparator: return "Consecutive digit separators detected.";
    case DigitSeparatorAdjacentToDecimalPoint:
      return "The decimal point cannot be preceded or followed by a digit separator.";
    case SingleColon: return "Did you mean '::'?";
    case InvalidNumericLiteral: return "Invalid numeric literal found.";
    case UnclosedStringLiteral: return "String literal was not closed.";
    case UnclosedCharacterLiteral: return "Character literal was not closed";
    default: break;
  }
  return "unrecognized error";
}
}  // namespace librepr::parsing::token::LexError

#pragma once
#include <cstdint>
#include <string_view>
#include <librepr/macro/assert.h>
#include <librepr/util/hash.h>
#include "token.h"
#include "classify.h"
#include "token/keywords.h"

namespace librepr::parsing {
// NOLINTBEGIN(*-pointer-arithmetic)
class Lexer {
public:
  std::string_view data;
  std::uint16_t length;
  std::uint16_t cursor = 0;

  constexpr explicit Lexer(std::string_view data) : data(data), length(data.length()) {
    // TODO assert data is small enough
  }

  constexpr Token next(bool peek = false) {
    if (data.empty() || cursor >= length) {
      // nothing to do, return sentinel
      return Token{nullptr, 0, token::Punctuation::eof};
    }

    auto const previous      = cursor;
    Token const return_value = next_token();
    if (peek) {
      // roll back cursor if we were supposed to peek
      cursor = previous;
    }
    return return_value;
  }

private:
  template <std::same_as<char>... Ts>
  [[nodiscard]] constexpr bool check(std::size_t offset, Ts... needles) const {
    if (cursor + offset >= length) {
      return false;
    }
    return ((data[cursor + offset] == needles) || ...);
  }

  template <std::same_as<char>... Ts>
  constexpr bool advance_if(Ts... needles) {
    if (cursor >= length) {
      return false;
    }

    if (((data[cursor] == needles) || ...)) {
      ++cursor;
      return true;
    }
    return false;
  }

  template <std::same_as<char>... Ts>
  constexpr bool advance_if_not(Ts... needles) {
    if (cursor >= length) {
      return false;
    }

    if (((data[cursor] != needles) && ...)) {
      ++cursor;
      return true;
    }
    return false;
  }

  template <typename F>
  constexpr bool advance_pred(F&& pred) {
    if (cursor >= length) {
      return false;
    }
    if (pred(data[cursor])) {
      ++cursor;
      return true;
    }
    return false;
  }

  constexpr void skip_whitespace() {
    while (cursor < length && is_whitespace(data[cursor])) {
      ++cursor;
    }
  }

  constexpr bool lex_numeric_head(Token& token) {
    if (!token.is(TokenCategory::numeric)) {
      // do not overwrite in-flight error
      if (!token.is(TokenCategory::error)) {
        token = token::LexError::InvalidNumericLiteral;
      }

      return false;
    }

    auto& flags = token.get<token::Numeral>();
    if (advance_if('0')) {
      if (advance_if('x', 'X')) {
        if (!advance_pred(is_hex)) {
          token = token::LexError::InvalidNumericLiteral;
          return false;
        }
      } else if (advance_if('b', 'B')) {
        if (!advance_if('0', '1')) {
          token = token::LexError::InvalidNumericLiteral;
          return false;
        }
        flags.base = token::Numeral::binary;
        return true;
      }
      // TODO ensure b/x is not followed by ' or .
      flags.base = token::Numeral::octal;
      return true;
    }
    flags.base = token::Numeral::decimal;
    return true;
  }

  constexpr Token lex_numeric(Token& token) {
    auto& flags = token.get<token::Numeral>();

    if (!flags.is_float()) {
      lex_numeric_head(token);
    }

    while (cursor < length) {
      if (advance_if('\'')) {
        if (check(0, '.')) {
          return Token{token.start, cursor, token::LexError::DigitSeparatorAdjacentToDecimalPoint};
        }
        if (check(0, '\'')) {
          return Token{token.start, cursor, token::LexError::ConsecutiveDigitSeparator};
        }
        continue;
      }

      if (flags.is(token::Numeral::binary)) {
        if (!advance_if('0', '1')) {
          break;
        }
      } else {
        if (advance_if('.')) {
          if (check(0, '\'')) {
            return Token{token.start, cursor, token::LexError::DigitSeparatorAdjacentToDecimalPoint};
          }
          if (flags.is_float()) {
            return Token{token.start, cursor, token::LexError::MultipleDecimalPoints};
          }
          flags.set(token::Numeral::double_);
          continue;
        }

        if (flags.is_float() && flags.is(token::Numeral::decimal)) {
          if (advance_if('E', 'e')) {
            continue;
          }
          if (!advance_pred(is_numeric)) {
            break;
          }
        } else if (flags.is_float() && flags.is(token::Numeral::hex)) {
          if (advance_if('P', 'p')) {
            continue;
          }
          if (!advance_pred(is_hex)) {
            break;
          }
        } else if (!advance_pred(is_numeric)) {
          break;
        }
      }
    }

    // consume literal suffix
    if (flags.is_float()) {
      if (advance_if('F', 'f')) {
        flags.set(token::Numeral::float_);
      }
    } else {
      // TODO set types
      if (advance_if('U', 'u')) {
        advance_if('L', 'l');
        advance_if('L', 'l');
      } else if (advance_if('L', 'l')) {
        advance_if('U', 'u');
        advance_if('L', 'l');
        advance_if('U', 'u');
      } else {
        advance_if('z', 'Z');
      }
    }
    return Token{token.start, cursor, flags};
  }

  constexpr Token lex_identifier(Token& token, bool preprocessor = false) {
    while (cursor < length && is_ident_continue(data[cursor])) {
      ++cursor;
    }
    token.end = cursor;
    if (auto const* entry = token::detail::KeywordHash::find(token.start, token.end)) {
      token = entry->value;
    } else {
      token = token::Name{token::Identifier::identifier};
    }

    return token;
  }

  constexpr Token lex_string(Token& token) {
    while (cursor < length && data[cursor] != '"') {
      ++cursor;
    }
    if (data[cursor] != '"') {
      return Token{token.start, cursor, token::LexError::UnclosedStringLiteral};
    }
    ++cursor;
    token.end = cursor;

    return token;
  }

  constexpr Token lex_character(Token& token) {
    advance_if('\\');
    ++cursor;
    if (!advance_if('\'')) {
      return Token{token.start, cursor, token::LexError::UnclosedCharacterLiteral};
    }
    token.end = cursor;
    return token;
  }

  constexpr Token next_token() {
    skip_whitespace();
    auto const start_cursor = cursor;
    if (cursor >= length) {
      // nothing to parse
      return Token{nullptr, 0, token::Punctuation::eof};
    }
    ++cursor;
    Token output = Token{&data[start_cursor], cursor, token::Punctuation::eof};
    switch (data[start_cursor]) {
      using enum token::Punctuation::Kind;
      case '\0':
        // found end of file, nothing to parse
        output.end = cursor;
        output     = eof;
        break;

      case '?': output = question; break;
      case '[': output = l_square; break;
      case ']': output = r_square; break;
      case '(': output = l_paren; break;
      case ')': output = r_paren; break;
      case '{': output = l_brace; break;
      case '}': output = r_brace; break;
      case '~': output = tilde; break;
      case ';': output = semi; break;
      case ',': output = comma; break;
      case '*': output = advance_if('=') ? starequal : star; break;
      case '!': output = advance_if('=') ? exclaimequal : exclaim; break;
      case '%': output = advance_if('=') ? percentequal : percent; break;
      case '^': output = advance_if('=') ? caretequal : caret; break;
      case ':': output = advance_if(':') ? coloncolon : colon; break;
      case '=': output = advance_if('=') ? equalequal : equal; break;
      case '"': output = token::StringFlags::is_plain; return lex_string(output);
      case '\'': output = token::CharacterFlags::is_plain; return lex_character(output);

      // clang-format off
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        --cursor;
        output = token::Numeral{};
        return lex_numeric(output);

      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':  case 'G': case 'H': case 'I': case 'J':
      case 'K': /*'L'*/   case 'M': case 'N': case 'O': case 'P': case 'Q': /*'R'*/    case 'S': case 'T': 
      /*'U'*/   case 'V': case 'W': case 'X': case 'Y': case 'Z': 
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': 
      case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
      /*'u'*/   case 'v': case 'w': case 'x': case 'y': case 'z': case '_': case '$':
        // clang-format on
        return lex_identifier(output);

      case 'u':
      case 'U':
        // TODO

      case 'L': {                             // wide literal or ident
        bool const is_raw = advance_if('R');  // wide raw literal or ident
        if (advance_if('"')) {
          output = is_raw ? (token::StringFlags::Flag)(token::StringFlags::is_wide | token::StringFlags::is_raw)
                          : token::StringFlags::is_wide;
          return lex_string(output);
        } else if (advance_if('\'')) {
          output = is_raw
                       ? (token::CharacterFlags::Flag)(token::CharacterFlags::is_wide | token::CharacterFlags::is_raw)
                       : token::CharacterFlags::is_wide;
          return lex_character(output);
        } else {
          return lex_identifier(output);
        }
      }

      case 'R':  // raw string literal or ident
        if (advance_if('"')) {
          output = token::StringFlags::is_raw;
          return lex_string(output);
        }
        return lex_identifier(output);

      case '.':
        if (check(1, '1', '2', '3', '4', '5', '6', '7', '8', '9')) {
          output = token::Numeral{};
          output.get<token::Numeral>().set(token::Numeral::double_);
          return lex_numeric(output);
        }
        output = advance_if('*') ? periodstar : period;
        break;

      case '&':
        if (advance_if('&')) {
          output = ampamp;
        } else {
          output = advance_if('=') ? ampequal : amp;
        }
        break;

      case '+':
        if (advance_if('+')) {
          output = plusplus;
        } else {
          output = advance_if('=') ? plusequal : plus;
        }
        break;

      case '-':
        if (advance_if('-')) {
          output = minusminus;
        } else if (advance_if('=')) {
          output = minusequal;
        } else if (advance_if('>')) {
          output = advance_if('*') ? arrowstar : arrow;
        } else if (advance_pred(is_numeric)) {
          // -1234
          output = token::Numeral{};
          output.get<token::Numeral>().set(token::Numeral::sign);
          return lex_numeric(output);
        } else {
          output = minus;
        }

        break;

      case '/':
        if (data[cursor] == '/') {
          while (advance_if_not('\n')) { /* advance until next line */
          }
          return next_token();
        }
        if (data[cursor] == '*') {
          // inline comment
          ++cursor;
          while (cursor < length) {
            if (data[cursor] == '*' && check(1, '/')) {
              cursor += 2;
              break;
            }
            ++cursor;
          }
          return next_token();
        }

        output = advance_if('=') ? slashequal : slash;
        break;

      case '<':
        if (advance_if('=')) {
          output = advance_if('>') ? spaceship : lessequal;
        } else if (advance_if('<')) {
          output = advance_if('=') ? lesslessequal : lessless;
        } else {
          output = less;
        }
        break;

      case '>':
        if (advance_if('=')) {
          output = greaterequal;
        } else if (advance_if('>')) {
          output = advance_if('=') ? greatergreaterequal : greatergreater;
        } else {
          output = greater;
        }
        break;

      case '|':
        if (advance_if('=')) {
          output = pipeequal;
        } else if (advance_if('|')) {
          output = pipepipe;
        } else {
          output = pipe;
        }
        break;

      default: output = token::LexError::UnknownCharacter; break;
    }

    return output;
  }
};
// NOLINTEND(*-pointer-arithmetic)
}  // namespace librepr::parsing
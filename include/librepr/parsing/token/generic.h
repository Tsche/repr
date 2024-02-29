#pragma once
namespace librepr::parsing::token::TokenKind {
enum Kind : unsigned short {
  invalid,
  eof,
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
}
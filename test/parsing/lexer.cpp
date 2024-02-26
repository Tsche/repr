#include <gtest/gtest.h>
#include <librepr/parsing/lex.h>

namespace librepr::parsing::test {
/*
namespace {
Token expect_one_token(std::string_view input, TokenKind kind) {
  auto lexer = Lexer(input);
  Token first_token = lexer.next();
 
  EXPECT_EQ(first_token.kind, kind);
  EXPECT_EQ(first_token.start, input.begin());
  EXPECT_EQ(first_token.end, input.end());
  EXPECT_EQ(first_token.size(), input.size());

  Token second_token = lexer.next();
  EXPECT_EQ(second_token.kind, TokenKind::eof);

  return first_token;
}
}

TEST(parsing_lex, empty){
  auto lexer = Lexer(std::string_view{});
  auto token = lexer.next();
  EXPECT_EQ(token.kind, TokenKind::eof);
  EXPECT_EQ(token.size(), 0);

  lexer = Lexer(" ");
  token = lexer.next();
  EXPECT_EQ(token.kind, TokenKind::eof);
  EXPECT_EQ(token.size(), 0);

  lexer = Lexer("\n");
  token = lexer.next();
  EXPECT_EQ(token.kind, TokenKind::eof);
  EXPECT_EQ(token.size(), 0);

  lexer = Lexer("\t");
  token = lexer.next();
  EXPECT_EQ(token.kind, TokenKind::eof);
  EXPECT_EQ(token.size(), 0);

  lexer = Lexer(" \n \t");
  token = lexer.next();
  EXPECT_EQ(token.kind, TokenKind::eof);
  EXPECT_EQ(token.size(), 0);
}

TEST(parsing_lex, identifier) {
  expect_one_token("foo", TokenKind::identifier);
  expect_one_token("foo_bar", TokenKind::identifier);
  expect_one_token("FOO", TokenKind::identifier);
  expect_one_token("FOO_BAR", TokenKind::identifier);
  expect_one_token("foo", TokenKind::identifier);

  expect_one_token("_zoinks", TokenKind::identifier);
  expect_one_token("_Zoinks", TokenKind::identifier);

  // special case L
  expect_one_token("L", TokenKind::identifier);
  expect_one_token("Lfoo", TokenKind::identifier);

  // special case R
  expect_one_token("R", TokenKind::identifier);
  expect_one_token("Rfoo", TokenKind::identifier);

  // special case U
  expect_one_token("u", TokenKind::identifier);
  expect_one_token("ufoo", TokenKind::identifier);

  // special case u
  expect_one_token("u", TokenKind::identifier);
  expect_one_token("ufoo", TokenKind::identifier);

  // special case $
  expect_one_token("$", TokenKind::identifier);
  expect_one_token("$foo", TokenKind::identifier);
}

TEST(parsing_lex, numeral) {

}

TEST(parsing_lex, string_literal) {

}

TEST(parsing_lex, character_literal) {

}

TEST(parsing_lex, braces) {

}

TEST(parsing_lex, symbols) {

}*/
}
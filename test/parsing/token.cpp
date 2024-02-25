#include <gtest/gtest.h>
#include "librepr/parsing/token/lex_error.h"
#include <librepr/parsing/token.h>
#include <librepr/parsing/token_kind.h>

namespace librepr::parsing::testing {

TEST(lex_token, default_) {
  auto token = Token(0, 0);
  EXPECT_TRUE(token.is(TokenCategory::error));
  EXPECT_TRUE(!token.in(TokenCategory::Category(~0U))); // error is not part of the valid set of tokens
}

TEST(lex_token, error) {
  auto token = Token(0, 0, LexError::UnclosedStringLiteral);
  EXPECT_TRUE(token.is(TokenCategory::error));
  // EXPECT_TRUE(!token.in(TokenCategory::Category(~0U))); // error is not part of the valid set of tokens

  EXPECT_TRUE(token.is(LexError::UnclosedStringLiteral));
  EXPECT_TRUE(token.in((LexError::Error)(LexError::UnclosedStringLiteral | LexError::UnclosedCharacterLiteral)));
}

TEST(lex_token, tokenkind) {
  auto token = Token(0, 0, TokenKind::l_brace);
  EXPECT_TRUE(token.is(TokenCategory::generic));
  // EXPECT_TRUE(token.in(TokenCategory::Category(~0U)));

  EXPECT_TRUE(token.is(TokenKind::l_brace));
  EXPECT_TRUE(token.in((TokenKind::Kind)(TokenKind::l_brace | TokenKind::r_brace)));
}

TEST(lex_token, character) {
  auto token = Token(0, 0, CharacterFlags::is_wide);
  EXPECT_TRUE(token.is(TokenCategory::character));
  // EXPECT_TRUE(token.in(TokenCategory::Category(~0U)));

  EXPECT_TRUE(token.is(CharacterFlags::is_wide));
  EXPECT_TRUE(token.in((CharacterFlags::Flag)(CharacterFlags::is_wide | CharacterFlags::is_raw)));
}

TEST(lex_token, string) {
  auto token = Token(0, 0, StringFlags::is_wide);
  EXPECT_TRUE(token.is(TokenCategory::string));
  // EXPECT_TRUE(token.in(TokenCategory::Category(~0U)));

  EXPECT_TRUE(token.is(StringFlags::is_wide));
  EXPECT_TRUE(token.in((StringFlags::Flag)(StringFlags::is_wide | StringFlags::is_raw)));
}

TEST(lex_token, numeral) {
  auto token = Token(0, 0, Numeral{.flags=Numeral::is_floating, .base=Numeral::hex, .kind={.floating=Numeral::float_}});
  EXPECT_TRUE(token.is(TokenCategory::numeric));
  // EXPECT_TRUE(token.in(TokenCategory::Category(~0U)));
  
  auto const& flags = token.get<Numeral>();
  EXPECT_TRUE(flags.is_float());
  EXPECT_TRUE(flags.is_signed()); // floats have a sign

  // EXPECT_TRUE(flags.is(Numeral::is_floating));
  // EXPECT_TRUE(flags.is(Numeral::hex));
  EXPECT_TRUE(flags.is(Numeral::float_));
}

TEST(lex_token, keyword) {
  auto token = Token(0, 0, Name{Name::type});
  EXPECT_TRUE(token.is(TokenCategory::name));
  // EXPECT_TRUE(token.in(TokenCategory::Category(~0U)));
  
  auto const& flags = token.get<Name>();
  EXPECT_TRUE(flags.kind == Name::type);
  EXPECT_TRUE(flags.is(Name::type));
}

TEST(lex_token, reassign) {
  auto token = Token();
  EXPECT_TRUE(token.is(TokenCategory::error));
  EXPECT_EQ(token.get<LexError::Error>(), LexError::Unknown);

  token = Numeral();
  EXPECT_TRUE(token.is(TokenCategory::numeric));

  token = StringFlags::is_wide;
  EXPECT_TRUE(token.is(TokenCategory::string));
  EXPECT_TRUE(token.is(StringFlags::is_wide));

  token = LexError::UnclosedStringLiteral;
  EXPECT_TRUE(token.is(TokenCategory::error));
  EXPECT_TRUE(token.is(LexError::UnclosedStringLiteral));

  token = CharacterFlags::is_raw;
  EXPECT_TRUE(token.is(TokenCategory::character));
  EXPECT_TRUE(token.is(CharacterFlags::is_raw));

  token = TokenKind::l_brace;
  EXPECT_TRUE(token.is(TokenCategory::generic));
  EXPECT_TRUE(token.is(TokenKind::l_brace));
}
}
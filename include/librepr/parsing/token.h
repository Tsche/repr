#pragma once
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <librepr/util/union.h>

#include <librepr/util/concepts.h>
#include "token_kind.h"
#include "token/lex_error.h"

namespace librepr::parsing {

enum class TokenCategory : unsigned char { error, generic, character, string, numeric, name };

template <auto V>
concept is_token_category = std::same_as<std::remove_const_t<decltype(V)>, TokenCategory>;

class Token {
private:
public:
  union Type {
    LexError::Error lex_error;
    TokenKind::Kind kind;
    CharacterFlags::Flag char_flags;
    StringFlags::Flag string_flags;
    Numeral numeric_flags;
    detail::Name name_flags;

    static_assert(util::EnableUnion<util::UnionMember{&Type::lex_error, TokenCategory::error},
                                    util::UnionMember{&Type::kind, TokenCategory::generic},
                                    util::UnionMember{&Type::char_flags, TokenCategory::character},
                                    util::UnionMember{&Type::string_flags, TokenCategory::string},
                                    util::UnionMember{&Type::numeric_flags, TokenCategory::numeric},
                                    util::UnionMember{&Type::name_flags, TokenCategory::name}>);
  } type{.lex_error = LexError::Unknown};
  TokenCategory category{TokenCategory::error};

public:
  uint16_t start;
  uint16_t end;
  constexpr Token() : start(0), end(0) {}
  constexpr Token(uint16_t start_, uint16_t end_) : start(start_), end(end_) {}

  template <typename T>
    requires util::is_tagged_union<Type, T>
  constexpr explicit Token(uint16_t start_, uint16_t end_, T value)
      : start(start_)
      , end(end_)
      , category(util::get_union_tag<Type, T>) {
    std::construct_at(&(type.*util::get_union_accessor<Type, util::get_union_tag<Type, T>>), value);
  }

  template <typename T>
    requires util::is_tagged_union<Type, T>
  constexpr Token& operator=(T value) {
    category = util::get_union_tag<Type, T>;
    std::construct_at(&(type.*util::get_union_accessor<Type, util::get_union_tag<Type, T>>), value);
    return *this;
  }

  template <typename T>
  [[nodiscard]] constexpr T& get() {
    constexpr TokenCategory tag = util::get_union_tag<Type, T>;

    if (tag == category) {
      return type.*util::get_union_accessor<Type, tag>;
    }
    throw std::runtime_error("Member not held");
  }

  template <typename T>
  [[nodiscard]] constexpr T const& get() const {
    constexpr TokenCategory tag = util::get_union_tag<Type, T>;

    if (tag == category) {
      return type.*util::get_union_accessor<Type, tag>;
    }
    throw std::runtime_error("Member not held");
  }

  [[nodiscard]] constexpr bool is(TokenCategory category_) const { return category == category_; }

  template <librepr::util::is_class T>
    requires is_token_category<T::tag>
  [[nodiscard]] constexpr bool is(T flags) const {
    if (category != T::tag) {
      return false;
    }

    T const& current = get<T>();
    return current.is(flags);
  }

  template <typename T>
    requires std::is_enum_v<T>
  [[nodiscard]] constexpr bool is(T flags) const {
    if (category != util::get_union_tag<Type, T>) {
      return false;
    }

    auto const& current = get<util::union_member_type<Type, T>>();
    static_assert(std::same_as<std::remove_cvref_t<decltype(current)>, T>);

    return current == flags;
  }

  [[nodiscard]] constexpr bool in(TokenCategory category_) const { return category_ == category; }

  template <std::same_as<TokenCategory>... Ts>
  [[nodiscard]] constexpr bool in(Ts... alternatives) const {
    return ((category == alternatives) || ...);
  }

  template <librepr::util::is_class T, librepr::util::is_class... Ts>
    requires is_token_category<T::tag> && (is_token_category<Ts::tag> && ...)
  [[nodiscard]] constexpr bool in(T flags) const {
    if (category != T::tag) {
      return false;
    }

    T const& current = get<T>();
    return current.has(flags);
  }

  template <librepr::util::is_enum T, std::same_as<T>... Ts>
    requires(!std::same_as<T, TokenCategory>)
  [[nodiscard]] constexpr bool in(T flag, Ts... flags) const {
    if (category != util::get_union_tag<Type, T>) {
      return false;
    }

    auto const& current = get<util::union_member_type<Type, T>>();
    static_assert(std::same_as<std::remove_cvref_t<decltype(current)>, T>);
    return (current & static_cast<T>((flag | ... | flags))) != 0;
  }

  [[nodiscard]] constexpr operator bool() const {
    if (category == TokenCategory::error) {
      return false;
    }

    if (category == TokenCategory::generic) {
      return !is(TokenKind::eof);
    }

    return true;
  }

  [[nodiscard]] constexpr std::string_view extract(std::string_view data) const {
    return data.substr(start, end - start);
  }
};

// static_assert(sizeof(Token) <= 8);
}  // namespace librepr::parsing

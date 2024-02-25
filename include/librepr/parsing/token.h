#pragma once
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <librepr/util/union.h>

#include <librepr/util/concepts.h>
#include "token_kind.h"
#include "lex_error.h"

namespace librepr::parsing {

template <auto V>
concept is_token_category = std::same_as<std::remove_const_t<decltype(V)>, TokenCategory::Category>;

class Token {
private:
public:
  TokenCategory::Category category{TokenCategory::error};
  union Type {
    LexError::Error lex_error;
    TokenKind::Kind kind;
    CharacterFlags::Flag char_flags;
    StringFlags::Flag string_flags;
    Numeral numeric_flags;
    Name name;

    static_assert(EnableUnion<&Type::lex_error,
                              &Type::kind,
                              &Type::char_flags,
                              &Type::string_flags,
                              &Type::numeric_flags,
                              &Type::name>);
  } type{.lex_error = LexError::Unknown};

public:
  uint16_t start;
  uint16_t end;
  constexpr Token() : start(0), end(0) {}
  constexpr Token(uint16_t start_, uint16_t end_) : start(start_), end(end_) {}

  /**
   * @brief Construct from class type union members
   *
   * @tparam T A class type that's a member of the Type union
   * @param start_
   * @param end_
   * @param type_
   */
  template <librepr::util::is_class T>
    requires is_token_category<T::tag>
  constexpr Token(uint16_t start_, uint16_t end_, T type_) : start(start_),
                                                             end(end_),
                                                             category(T::tag) {
    std::construct_at(&(type.*get_union_accessor<Type, T::tag>), type_);
  }

  /**
   * @brief Construct from enum type union members
   *
   * @tparam T An enum type that's a member of the Type union
   * @param start_
   * @param end_
   * @param type_
   */
  template <librepr::util::is_enum T>
    requires is_token_category<get_tag<T>>
  constexpr Token(uint16_t start_, uint16_t end_, T type_) : start(start_),
                                                             end(end_),
                                                             category(get_tag<T>) {
    std::construct_at(&(type.*get_union_accessor<Type, get_tag<T>>), type_);
  }

  template <librepr::util::is_class T>
    requires is_token_category<T::tag>
  constexpr Token& operator=(T type_) {
    category = T::tag;
    std::construct_at(&(type.*get_union_accessor<Type, T::tag>), type_);
    return *this;
  }

  template <librepr::util::is_enum T>
    requires is_token_category<get_tag<T>>
  constexpr Token& operator=(T type_) {
    category = get_tag<T>;
    std::construct_at(&(type.*get_union_accessor<Type, get_tag<T>>), type_);
    return *this;
  }

  template <typename T>
  [[nodiscard]] constexpr T& get() {
    constexpr TokenCategory::Category tag = get_tag<T>;

    if (tag == category) {
      return type.*get_union_accessor<Type, tag>;
    }
    throw std::runtime_error("Member not held");
  }

  template <typename T>
  [[nodiscard]] constexpr T const& get() const {
    constexpr TokenCategory::Category tag = get_tag<T>;

    if (tag == category) {
      return type.*get_union_accessor<Type, tag>;
    }
    throw std::runtime_error("Member not held");
  }

  [[nodiscard]] constexpr bool is(TokenCategory::Category category_) const { return category == category_; }

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
    if (category != get_tag<T>) {
      return false;
    }

    auto const& current = get<get_union_type<Type, get_tag<T>>>();
    static_assert(std::same_as<std::remove_cvref_t<decltype(current)>, T>);

    return current == flags;
  }

  [[nodiscard]] constexpr bool in(TokenCategory::Category category_) const { return category_ == category; }

  template <std::same_as<TokenCategory::Category>... Ts>
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
    requires(!std::same_as<T, TokenCategory::Category>)
  [[nodiscard]] constexpr bool in(T flag, Ts... flags) const {
    if (category != get_tag<T>) {
      return false;
    }

    auto const& current = get<get_union_type<Type, get_tag<T>>>();
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

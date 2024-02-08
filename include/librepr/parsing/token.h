#pragma once
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <librepr/util/union.h>

#include "token_kind.h"
#include "lex_error.h"

namespace librepr::parsing {

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

    static_assert(
        EnableUnion<&Type::lex_error, &Type::kind, &Type::char_flags, &Type::string_flags, &Type::numeric_flags>);
  } type{.lex_error = LexError::Unknown};

public:
  uint16_t start;
  uint16_t end;

  constexpr Token(uint16_t start_, uint16_t end_) : start(start_), end(end_) {}

  /**
   * @brief Construct from class type union members
   *
   * @tparam T A class type that's a member of the Type union
   * @param start_
   * @param end_
   * @param type_
   */
  template <typename T>
    requires std::is_class_v<T> && std::same_as<decltype(T::tag), TokenCategory::Category const>
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
  template <typename T>
    requires std::is_enum_v<T> && std::same_as<decltype(get_tag<T>), TokenCategory::Category const>
  constexpr Token(uint16_t start_, uint16_t end_, T type_) : start(start_),
                                                             end(end_),
                                                             category(get_tag<T>) {
    std::construct_at(&(type.*get_union_accessor<Type, get_tag<T>>), type_);
  }

  template <typename T>
    requires std::is_class_v<T> && std::same_as<decltype(T::tag), TokenCategory::Category const>
  constexpr Token& operator=(T type_) {
    category = T::tag;
    std::construct_at(&(type.*get_union_accessor<Type, T::tag>), type_);
    return *this;
  }

  template <typename T>
    requires std::is_enum_v<T> && std::same_as<decltype(get_tag<T>), TokenCategory::Category const>
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

  template <typename T>
    requires std::is_class_v<T> && std::same_as<decltype(T::tag), TokenCategory::Category const>
  [[nodiscard]] constexpr bool is(T flags) const {
    T const& current = get<T>();
    return current.is(flags);
  }

  template <typename T>
    requires std::is_enum_v<T>
  [[nodiscard]] constexpr bool is(T flags) const {
    auto const& current = get<get_union_type<Type, get_tag<T>>>();
    static_assert(std::same_as<std::remove_cvref_t<decltype(current)>, T>);

    return current == flags;
  }

  [[nodiscard]] constexpr bool in(TokenCategory::Category category_) const { return (category & category_) != 0; }

  template <typename T>
    requires std::is_class_v<T> && std::same_as<decltype(T::tag), TokenCategory::Category const>
  [[nodiscard]] constexpr bool in(T flags) const {
    T const& current = get<T>();
    return current.has(flags);
  }

  template <typename T>
    requires std::is_enum_v<T>
  [[nodiscard]] constexpr bool in(T flags) const {
    auto const& current = get<get_union_type<Type, get_tag<T>>>();
    static_assert(std::same_as<std::remove_cvref_t<decltype(current)>, T>);
    return (current & flags) != 0;
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
};
}  // namespace librepr::parsing

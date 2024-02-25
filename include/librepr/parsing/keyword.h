#pragma once
#include <string_view>
#include <librepr/customization.h>
#include <librepr/util/union.h>
#include <librepr/util/concepts.h>
#include "token/generic.h"

namespace librepr::parsing::detail {

enum class Literal : unsigned char {
  True,
  False,
  Nullptr,
};

namespace Qualifier {
enum CV : unsigned char {
  Const    = 1U << 0U,
  Volatile = 1U << 1U,
};
}

enum class AccessSpecifier : unsigned char { Private, Protected, Public };

namespace Specifier {
enum Spec : unsigned char {
  Consteval,
  Constexpr,
  Constinit,
  Inline,
  Mutable,
};
}

enum class ClassKey : unsigned char {
  Class,
  Struct,
  Union,
};

enum class EnumKey : unsigned char { Enum, EnumClass, EnumStruct };

enum class Keyword : unsigned char {
  Template,
  Typename,
  Concept,
  Namespace,
  Export,
  Extern,
  Friend,
  Operator,
  Register,
  Static,
  This,
  ThreadLocal,
  Typedef,
  Using,
  Virtual,
  Delete,
  New,
};

enum class WindowsKeyword : unsigned char {
  Asm,
  Cdecl,
  Restrict,
  FastCall,
  ThisCall,
  StdCall,
  VectorCall,
  W64,
  Unaligned,
  Sptr,
  Uptr,
  Event,
  Inline,
  ForceInline,
  Interface,
  SingleInheritance,
  MultipleInheritance,
  VirtualInheritance,
  Raise,
  Super,
};

enum class Control : unsigned char {
  Return,
  If,
  Else,
  For,
  Continue,
  While,
  Do,
  Throw,
  Try,
  Catch,
  Switch,
  Case,
  Default,
  Break,
  Goto,
  CoAwait,
  CoYield,
  CoReturn,
};

enum class WindowsSEH : unsigned char {
  Try,
  Except,
  Leave,
  Finally,
};

enum class Cast : unsigned char {
  Const,
  Dynamic,
  Reinterpret,
  Static,
};

enum class WindowsMagic : unsigned char {
  Declspec,
  Based,
  Assume,
  Alignof,
  IfNotExists,
  IfExists,
  Hook,
  Unhook,
  UuidOf,
};

enum class Magic : unsigned char {
  Asm,
  Requires,
  Sizeof,
  StaticAssert,
  Typeid,
  Alignas,
  Alignof,
  Explicit,
  Noexcept,
  Decltype,
};

enum class Type : unsigned char {
  Int8,
  Int16,
  Int32,
  Int64,
  M64,
  M128,
  M128d,
  M128i,
  Ptr32,
  Ptr64,
  Auto,
  Bool,
  Char,
  Char8,
  Char16,
  Char32,
  Double,
  Float,
  Int,
  Void,
};

enum class Modifier : unsigned char {
  Long,
  Short,
  Signed,
  Unsigned,
};

enum class Operator : unsigned char {
  And    = TokenKind::ampamp,
  AndEq  = TokenKind::ampequal,
  BitAnd = TokenKind::amp,
  BitOr  = TokenKind::pipe,
  Compl  = TokenKind::tilde,
  Not    = TokenKind::exclaim,
  NotEq  = TokenKind::exclaimequal,
  Or     = TokenKind::pipepipe,
  OrEq   = TokenKind::pipeequal,
  Xor    = TokenKind::caret,
  XorEq  = TokenKind::caretequal,
};

enum class Contextual : unsigned char {
  Final,
  Override,
  Import,
  Module,
};

enum class Identifier : bool { 
  error = false,
  identifier = true
};

enum class NameCategory : unsigned char {
  identifier = 0,
  literal,
  qualifier,
  access_specifier,
  specifier,
  classkey,
  enumkey,
  keyword,
  control,
  cast,
  magic,
  type,
  modifier,
  op,
  contextual,
  // preprocessor ?
  windows_keyword,
  windows_magic,
  windows_seh,
};

template <auto V>
concept is_name_category = std::same_as<std::remove_const_t<decltype(V)>, NameCategory>;
}

//TODO find a better solution!

template <>
struct librepr::Settings<librepr::parsing::detail::Identifier> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::identifier;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Literal> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::literal;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Qualifier::CV> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::qualifier;
};

template <>
struct librepr::Settings<librepr::parsing::detail::AccessSpecifier> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::access_specifier;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Specifier::Spec> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::specifier;
};

template <>
struct librepr::Settings<librepr::parsing::detail::ClassKey> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::classkey;
};

template <>
struct librepr::Settings<librepr::parsing::detail::EnumKey> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::enumkey;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Keyword> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::keyword;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Control> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::control;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Cast> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::cast;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Magic> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::magic;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Type> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::type;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Modifier> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::modifier;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Operator> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::op;
};

template <>
struct librepr::Settings<librepr::parsing::detail::Contextual> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::contextual;
};

template <>
struct librepr::Settings<librepr::parsing::detail::WindowsKeyword> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::windows_keyword;
};

template <>
struct librepr::Settings<librepr::parsing::detail::WindowsMagic> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::windows_magic;
};

template <>
struct librepr::Settings<librepr::parsing::detail::WindowsSEH> {
  static constexpr auto tag = librepr::parsing::detail::NameCategory::windows_seh;
};

namespace librepr::parsing::detail {

struct Name {
  union Kind {
    Identifier identifier_;
    Literal literal_;
    Qualifier::CV qualifier_;
    AccessSpecifier access_;
    Specifier::Spec specifier_;
    ClassKey classkey_;
    EnumKey enumkey_;
    Keyword keyword_;
    Control control_;
    Cast cast_;
    Magic magic_;
    Type type_;
    Modifier modifier_;
    Operator operator_;
    Contextual contextual_;
    WindowsKeyword windows_keyword_;
    WindowsMagic windows_magic_;
    WindowsSEH windows_seh_;

    static_assert(librepr::EnableUnion<&Kind::identifier_ ,
                                       &Kind::literal_,
                                       &Kind::qualifier_,
                                       &Kind::access_,
                                       &Kind::specifier_,
                                       &Kind::classkey_,
                                       &Kind::enumkey_,
                                       &Kind::keyword_,
                                       &Kind::control_,
                                       &Kind::cast_,
                                       &Kind::magic_,
                                       &Kind::type_,
                                       &Kind::modifier_,
                                       &Kind::operator_,
                                       &Kind::contextual_,
                                       &Kind::windows_keyword_,
                                       &Kind::windows_magic_,
                                       &Kind::windows_seh_
                                       >);
  };

  Kind value {.identifier_=Identifier::error};
  NameCategory category {NameCategory::identifier};

  /*template <librepr::util::is_class T>
    requires is_name_category<T::tag>
  constexpr explicit Name(T value_) : category(T::tag) {
    std::construct_at(&(value.*get_union_accessor<Kind, T::tag>), value_);
  }*/

  template <librepr::util::is_enum T>
    requires is_name_category<get_tag<T>>
  constexpr explicit Name(T value_) : category(get_tag<T>) {
    std::construct_at(&(value.*get_union_accessor<Kind, get_tag<T>>), value_);
  }
};

struct KeywordEntry {
  std::string_view key;
  Name value;
};
}  // namespace librepr::parsing::detail

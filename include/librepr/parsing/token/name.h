#pragma once
#include <string_view>
#include <librepr/util/union.h>
#include <librepr/util/concepts.h>
#include "generic.h"

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

    static_assert(util::EnableUnion<util::UnionMember{&Kind::identifier_, NameCategory::identifier},
                                    util::UnionMember{&Kind::literal_, NameCategory::literal},
                                    util::UnionMember{&Kind::qualifier_, NameCategory::qualifier},
                                    util::UnionMember{&Kind::access_, NameCategory::access_specifier},
                                    util::UnionMember{&Kind::specifier_, NameCategory::specifier},
                                    util::UnionMember{&Kind::classkey_, NameCategory::classkey},
                                    util::UnionMember{&Kind::enumkey_, NameCategory::enumkey},
                                    util::UnionMember{&Kind::keyword_, NameCategory::keyword},
                                    util::UnionMember{&Kind::control_, NameCategory::control},
                                    util::UnionMember{&Kind::cast_, NameCategory::cast},
                                    util::UnionMember{&Kind::magic_, NameCategory::magic},
                                    util::UnionMember{&Kind::type_, NameCategory::type},
                                    util::UnionMember{&Kind::modifier_, NameCategory::modifier},
                                    util::UnionMember{&Kind::operator_, NameCategory::op},
                                    util::UnionMember{&Kind::contextual_, NameCategory::contextual},
                                    util::UnionMember{&Kind::windows_keyword_, NameCategory::windows_keyword},
                                    util::UnionMember{&Kind::windows_magic_, NameCategory::windows_magic},
                                    util::UnionMember{&Kind::windows_seh_, NameCategory::windows_seh}>);
  };

  Kind value {.identifier_=Identifier::error};
  NameCategory category {NameCategory::identifier};

  constexpr Name() = default;

  template <typename T>
    requires util::is_tagged_union<Kind, T>
  constexpr explicit Name(T value_) : category(util::get_union_tag<Kind, T>) {
    std::construct_at(&(value.*util::get_union_accessor<Kind, util::get_union_tag<Kind, T>>), value_);
  }

  [[nodiscard]] constexpr bool is(NameCategory type_) const { return category == type_; }
  [[nodiscard]] constexpr bool in(std::same_as<NameCategory> auto... types) const {
    return ((category == types) || ...);
  }
};

static_assert(sizeof(Name) <= 4);

struct KeywordEntry {
  const char* key;
  Name value;
};
}  // namespace librepr::parsing::detail

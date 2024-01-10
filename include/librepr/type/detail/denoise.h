#pragma once

/**
 * Some of these functions are unused, 
 * but I'm leaving them in so it's 
 * easier to extend later.
 */

#include <cstddef>
#include <set>
#include <type_traits>
#include <librepr/util/string_buffer.h>

namespace librepr::detail {
namespace msvc {
// For packing pos/len pairs
using uhalfptr_t = 
  std::conditional_t<sizeof(void*) == 8,
    std::uint32_t, std::uint16_t>;
// Extended into the signed world
using ihalfptr_t = std::make_signed_t<uhalfptr_t>;
using SymBuffer = StringBuffer;

struct SymToken {
  ihalfptr_t loc = -1;
  uhalfptr_t length = 0;
  explicit LIBREPR_HINT_INLINE operator bool() const { 
    return (loc >= 0); 
  }
};

struct SymLexer {
protected:
  SymLexer(std::string_view sym) 
   : begin(sym.data()), cursor(begin),
   buf(sym), nest_depth(), current_lexeme(), last_lexeme() {
    this->end = begin + sym.size();
  }

  static LIBREPR_HINT_INLINE bool isWord(char c) {
    return (c >= 'A' && c <= 'Z') ||
      (c >= 'a' && c <= 'z') ||
      (c == '_');
  }

  static LIBREPR_HINT_INLINE bool isQualifiedName(char c) {
    return isWord(c) || (c == ':') || (c >= '0' && c <= '9');
  }

  SymToken next() noexcept {
    last_lexeme = current_lexeme;
    cursor += last_lexeme.length;
    auto pos = advance();
    if(this->cursor == end) [[unlikely]] {
      return {};
    } else if(isWord(*cursor)) {
      auto epos = find_nonword();
      auto len = uhalfptr_t(epos - pos);
      this->current_lexeme = SymToken{pos, len};
    } else {
      this->current_lexeme = SymToken{pos, 1};
    }
    return this->current_lexeme;
  }

  LIBREPR_HINT_INLINE SymToken current() const noexcept {
    return this->current_lexeme;
  }

  LIBREPR_HINT_INLINE SymToken last() const noexcept {
    return this->last_lexeme;
  }

  LIBREPR_HINT_INLINE char current_first() const noexcept {
    return this->begin[current_lexeme.loc];
  }

  LIBREPR_HINT_INLINE char last_first() const noexcept {
    if(last_lexeme.loc < 0) [[unlikely]] {
      return '\0';
    }
    return this->begin[last_lexeme.loc];
  }

  char peek_first() const noexcept {
    auto pos = find_next(cursor + current_lexeme.length);
    if(pos == -1) [[unlikely]] {
      return '\0';
    }
    return this->begin[pos];
  }

  LIBREPR_HINT_INLINE void write(SymToken tok) {
    buf.write(to_sv(tok));
  }

  std::string_view to_sv(SymToken tok) const {
    auto* lloc = begin + tok.loc;
    LIBREPR_ASSERT(tok.length > 0, 
      "Invalid token length {}.", tok.length);
    return {lloc, lloc + tok.length};
  }

private:
  ihalfptr_t advance() {
    auto pos = find_next();
    if(pos != -1) {
      cursor = begin + pos;
    } else {
      cursor = end;
    }
    return ihalfptr_t(pos);
  }

  // Locates the next offset from begin
  std::intptr_t find_next(
   const char* pos = nullptr) const {
    auto* lloc = (!pos) ? cursor : pos;
    if(lloc == end) return -1;
    LIBREPR_ASSERT((lloc >= begin && lloc < end), 
      "Invalid position!");
    for(; lloc < end; ++lloc) {
      char c = *lloc;
      if(c != ' ') {
        return std::intptr_t(lloc - begin);
      }
    }
    return -1;
  }

  // Locates the next offset from begin
  std::intptr_t find_nonword() const {
    auto* lloc = cursor;
    for(; lloc < end; ++lloc) {
      if(!SymLexer::isQualifiedName(*lloc)) {
        return std::intptr_t(lloc - begin);
      }
    }
    return std::intptr_t(end - begin);
  }

private:
  const char* begin;
  const char* cursor;
  const char* end;
  SymToken current_lexeme;
  SymToken last_lexeme;

protected:
  SymBuffer buf;
  ihalfptr_t nest_depth = 0;
};

/// Does the actual formatting.
struct SymParser final : SymLexer {
  SymParser(std::string_view sym) 
    : SymLexer(sym) {
  }

  void parse() {
    while(auto tok = SymLexer::next()) {
      if(char c = SymLexer::current_first();
        !SymLexer::isWord(c)) 
      {
        this->handle_symbol(c);
        buf.write(c);
      } else if(!is_keyword(tok)) {
        this->handle_ident(tok);
        if(this->is_replacement(tok)) [[unlikely]] {
          continue;
        }
        SymLexer::write(tok);
      }
    }
  }

  std::string&& extract() {
    return std::move(SymLexer::buf.extract());
  }

private:
  LIBREPR_HINT_INLINE char last_written() {
    if(buf->empty()) [[unlikely]] {
      return '\0';
    }
    return buf->back();
  }

  LIBREPR_HINT_INLINE void handle_symbol(char c) {
    char lc = SymLexer::last_first();
    if(c == '>') {
      if(lc == '>') {
        buf.write(' ');
      }
      --SymLexer::nest_depth;
      LIBREPR_ASSERT(SymLexer::nest_depth >= 0,
        "Invalid nest level {}.", SymLexer::nest_depth);
    } else if(c == '<') {
      ++SymLexer::nest_depth;
    }
  }

  LIBREPR_HINT_INLINE void handle_ident(SymToken) {
    char lc = last_written();
    if(SymLexer::isQualifiedName(lc)) {
      buf.write(' ');
    } else if(SymLexer::nest_depth > 0) {
      if(lc == ',') buf.write(' ');
    }
  }

  bool is_keyword(SymToken tok) {
    static std::set<std::string_view> keywords {
      "struct", "class", "union", "enum",
      "__ptr64", "__cdecl", "__stdcall",
      "__thiscall", "__fastcall", ""
    };
    auto tstr = SymLexer::to_sv(tok);
    return keywords.contains(tstr);
  }

  bool is_replacement(SymToken tok) {
    auto tstr = SymLexer::to_sv(tok);
    if(tstr == "std::nullptr_t") {
      buf.write("decltype(nullptr)");
      return true;
    } else if(tstr == "__int64") {
      buf.write("long long");
      return true;
    }
    return false;
  }
};

}  // namespace msvc

/// Formats undecorated symbols to match itanium's symbols
[[nodiscard]] LIBREPR_HINT_INLINE std::string denoise_name(std::string_view name) {
  msvc::SymParser parser{name};
  parser.parse();
  return std::move(parser.extract());
}
}  // namespace librepr::detail

static_assert(sizeof(void*) == 
  sizeof(librepr::detail::msvc::SymToken));
#pragma once

#include <cstddef>
#include <cstring>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <librepr/util/strings.h>

#if defined(_DEBUG)
#define REPR_MSC_XASSERT(...) \
  _assert((REPR_FORMAT(__VA_ARGS__)).c_str(), (__FILE__), unsigned(__LINE__))
#define REPR_MSC_ASSERT(cond, ...) \
  do { if(!(cond)) [[unlikely]] { REPR_MSC_XASSERT(__VA_ARGS__); } } while(0)
#define REPR_MINLINE __forceinline
#else
#define REPR_MSC_XASSERT(...) (void)(0)
#define REPR_MSC_ASSERT(...) (void)(0)
#define REPR_MINLINE inline
#endif

#define RRemovePrefix(...) remove_prefix(REPR_STRSIZE(__VA_ARGS__))
#define RRemoveSuffix(...) remove_suffix(REPR_STRSIZE(__VA_ARGS__))

_CRT_BEGIN_C_HEADER
__declspec(dllimport) void 
 __cdecl _assert( // NOLINT
  char const* _Message,
  char const* _File,
  unsigned    _Line
);
_CRT_END_C_HEADER

namespace librepr::detail {
namespace msvc {
// For packing pos/len pairs
using uhalfptr_t = 
  std::conditional_t<sizeof(void*) == 8,
    std::uint32_t, std::uint16_t>;
// Extended into the signed world
using ihalfptr_t = std::make_signed_t<uhalfptr_t>;
using StringSet = std::set<std::string_view>;

// Wrapper around std::string.
struct SymBuffer {
  SymBuffer(std::string_view in) {
    // We can hopefully avoid reallocations
    // by doing this ahead of time.
    buf.reserve(in.size() * 2);
  }

  void write(const char* beg, const char* end) {
    REPR_MSC_ASSERT(end >= beg, "Invalid write. (end < beg)");
    if(auto len = (end - beg); len > 0) {
      auto lpos = buf.size();
      buf.resize(lpos + len);
      std::memcpy(buf.data() + lpos, beg, len);
    }
  }

  void write(std::string_view sv) {
    auto* beg = sv.data();
    this->write(beg, beg + sv.size());
  }

  void write(char c) {
    buf.push_back(c);
  }

  std::string&& extract() {
    // buf.shrink_to_fit();
    return std::move(buf);
  }

  REPR_MINLINE const std::string* 
   operator->() const {
    return &buf;
  }

private:
  std::string buf;
};

struct SymToken {
  ihalfptr_t loc = -1;
  uhalfptr_t len = 0;
  explicit REPR_MINLINE operator bool() const { 
    return (loc >= 0); 
  }
};

struct SymLexer {
protected:
  SymLexer(std::string_view sym) 
   : begin(sym.data()), loc(begin),
   buf(sym), nest_depth(), lcurr(), llast() {
    this->end = begin + sym.size();
  }

  static REPR_MINLINE bool isWord(char c) {
    return (c >= 'A' && c <= 'Z') ||
      (c >= 'a' && c <= 'z') ||
      (c == '_');
  }

  static REPR_MINLINE bool isIdent(char c) {
    return isWord(c) || (c == ':') || (c >= '0' && c <= '9');
  }

  SymToken next() noexcept {
    llast = lcurr;
    loc += llast.len;
    auto pos = advance();
    if(this->loc == end) [[unlikely]] {
      return {};
    } else if(isWord(*loc)) {
      auto epos = find_nonword();
      auto len = uhalfptr_t(epos - pos);
      this->lcurr = SymToken{pos, len};
    } else {
      this->lcurr = SymToken{pos, 1};
    }
    return this->lcurr;
  }

  REPR_MINLINE SymToken curr() const noexcept {
    return this->lcurr;
  }

  REPR_MINLINE SymToken last() const noexcept {
    return this->llast;
  }

  REPR_MINLINE char curr_first() const noexcept {
    return this->begin[lcurr.loc];
  }

  REPR_MINLINE char last_first() const noexcept {
    if(llast.loc < 0) [[unlikely]] {
      return '\0';
    }
    return this->begin[llast.loc];
  }

  char peek_first() const noexcept {
    auto pos = find_next(loc + lcurr.len);
    if(pos == -1) [[unlikely]] {
      return '\0';
    }
    return this->begin[pos];
  }

  REPR_MINLINE void write(SymToken tok) {
    buf.write(to_sv(tok));
  }

  std::string_view to_sv(SymToken tok) const {
    auto* lloc = begin + tok.loc;
    REPR_MSC_ASSERT(tok.len > 0, 
      "Invalid token length {}.", tok.len);
    return {lloc, lloc + tok.len};
  }

private:
  ihalfptr_t advance() {
    auto pos = find_next();
    if(pos != -1) {
      loc = begin + pos;
    } else {
      loc = end;
    }
    return ihalfptr_t(pos);
  }

  // Locates the next offset from begin
  std::intptr_t find_next(
   const char* pos = nullptr) const {
    auto* lloc = (!pos) ? loc : pos;
    if(lloc == end) return -1;
    REPR_MSC_ASSERT((lloc >= begin && lloc < end), 
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
    auto* lloc = loc;
    for(; lloc < end; ++lloc) {
      if(!SymLexer::isIdent(*lloc)) {
        return std::intptr_t(lloc - begin);
      }
    }
    return std::intptr_t(end - begin);
  }

private:
  const char* begin;
  const char* loc;
  const char* end;
  SymToken lcurr;
  SymToken llast;

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
      if(char c = SymLexer::curr_first();
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
  REPR_MINLINE char last_written() {
    if(buf->empty()) [[unlikely]] {
      return '\0';
    }
    return buf->back();
  }

  void handle_symbol(char c) {
    char lc = SymLexer::last_first();
    if(c == '>') {
      if(lc == '>')
        buf.write(' ');
      --SymLexer::nest_depth;
      REPR_MSC_ASSERT(SymLexer::nest_depth >= 0,
        "Invalid nest level {}.", SymLexer::nest_depth);
    } else if(c == '<') {
      ++SymLexer::nest_depth;
    }
  }

  void handle_ident(SymToken) {
    char lc = last_written();
    if(SymLexer::isIdent(lc)) {
      buf.write(' ');
    } else if(SymLexer::nest_depth > 0) {
      if(lc == ',') buf.write(' ');
    }
  }

  bool is_keyword(SymToken tok) {
    static StringSet keywords {
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
[[nodiscard]] inline std::string denoise_name(std::string_view name) {
  msvc::SymParser parser{name};
  parser.parse();
  return std::move(parser.extract());
}
}  // namespace librepr::detail

static_assert(sizeof(void*) == 
  sizeof(librepr::detail::msvc::SymToken));
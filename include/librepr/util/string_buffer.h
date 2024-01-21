#pragma once

#include <cstring>
#include <string>
#include <string_view>
#include <librepr/macro/assert.h>
#include <librepr/macro/util.h>
#include <librepr/util/strings.h>

namespace librepr::detail {
/// Wrapper around std::string.
struct StringBuffer {
  StringBuffer() = default;

  explicit StringBuffer(std::string_view in) {
    // We can hopefully avoid reallocations
    // by doing this ahead of time.
    buffer.reserve(in.size() * 2);
  }

  void write(const char* beg, const char* end) {
    LIBREPR_ASSERT(end >= beg, "Invalid write. (end < beg)");
    auto& target = (cursor == std::string::npos) ? buffer : inplace_buffer;
    if(auto len = (end - beg); len > 0) {
      auto lpos = target.size();
      target.resize(lpos + len);
      std::memcpy(target.data() + lpos, beg, len);
    }
  }

  LIBREPR_HINT_INLINE void write(std::string_view sv) {
    auto* beg = sv.data();
    this->write(beg, beg + sv.size());
  }

  LIBREPR_HINT_INLINE void write(char c) {
    auto& target = (cursor == std::string::npos) ? buffer : inplace_buffer;
    target.push_back(c);
  }

  LIBREPR_HINT_INLINE std::string&& extract() {
    flush();
    // buf.shrink_to_fit();
    return std::move(buffer);
  }

  explicit operator std::string() {
    flush();
    return buffer;
  }

  LIBREPR_HINT_INLINE const std::string* operator->() const {
    return &buffer;
  }

  friend StringBuffer& operator<<(StringBuffer& buf, std::string_view sv){
    buf.write(sv);
    return buf;
  }

  friend StringBuffer& operator<<(StringBuffer& buf, char c){
    buf.write(c);
    return buf;
  }

  friend std::ostream& operator<<(std::ostream& stream, StringBuffer& buf){
    buf.flush();
    return stream << buf.buffer;
  }
 
  std::size_t size() {
    return buffer.size() + inplace_buffer.size();
  }

  void set_cursor(std::size_t index = std::string::npos) {
    flush();
    cursor = index;
  }

  void flush() {
    if (!inplace_buffer.empty()) {
        // flush inplace buffer if it's not empty
        buffer.insert(cursor, inplace_buffer);
        inplace_buffer.clear();
    }
  }

private:
  std::string buffer{};
  std::string inplace_buffer{};
  std::size_t cursor{std::string::npos};
};
} // namespace librepr::detail

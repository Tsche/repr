#pragma once
#include <cstdint>
#include <cstring>
#include <iosfwd>
#include <string>
#include <string_view>
#include <utility>

#include <librepr/macro/assert.h>
#include <librepr/macro/util.h>

namespace librepr::util {
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

  LIBREPR_HINT_INLINE void write(std::string_view str) {
    auto* beg = str.data();
    this->write(beg, beg + str.size());
  }

  LIBREPR_HINT_INLINE void write(char character) {
    auto& target = (cursor == std::string::npos) ? buffer : inplace_buffer;
    target.push_back(character);
  }

  LIBREPR_HINT_INLINE void write(char character, std::uint16_t repeat){
    auto& target = (cursor == std::string::npos) ? buffer : inplace_buffer;
    target.append(repeat, character);
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

  friend StringBuffer& operator<<(StringBuffer& self, std::string_view str){
    self.write(str);
    return self;
  }

  friend StringBuffer& operator<<(StringBuffer& self, char character){
    self.write(character);
    return self;
  }

  friend std::ostream& operator<<(std::ostream& stream, StringBuffer& self){
    self.flush();
    return stream << self.buffer;
  }
 
  [[nodiscard]] std::size_t size() const {
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

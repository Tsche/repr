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
  StringBuffer(std::string_view in) {
    // We can hopefully avoid reallocations
    // by doing this ahead of time.
    buffer.reserve(in.size() * 2);
  }

  void write(const char* beg, const char* end) {
    LIBREPR_ASSERT(end >= beg, "Invalid write. (end < beg)");
    if(auto len = (end - beg); len > 0) {
      auto lpos = buffer.size();
      buffer.resize(lpos + len);
      std::memcpy(buffer.data() + lpos, beg, len);
    }
  }

  LIBREPR_HINT_INLINE void write(std::string_view sv) {
    auto* beg = sv.data();
    this->write(beg, beg + sv.size());
  }

  LIBREPR_HINT_INLINE void write(char c) {
    buffer.push_back(c);
  }

  LIBREPR_HINT_INLINE std::string&& extract() {
    // buf.shrink_to_fit();
    return std::move(buffer);
  }

  LIBREPR_HINT_INLINE const std::string* 
   operator->() const {
    return &buffer;
  }

private:
  std::string buffer;
};
} // namespace librepr::detail

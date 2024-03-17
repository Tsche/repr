#pragma once

#include <cstdint>
#include <format>
#include <librepr/macro/assert.h>
#include "buffer.h"

namespace librepr::util {
struct Printer : StringBuffer {
  struct Config {
    std::uint16_t max_width    = 80;
    std::uint16_t offset       = 0;
    std::uint8_t indent_amount = 4;
    bool break_before_curly    = false;
    bool break_after_curly     = true;
    const char* newline        = "\n";
  };
  enum Marker : std::uint8_t {
    endl,
    separator,
  };

  enum BlockKind : std::uint8_t { parens, braces, angles, brackets };

  Printer() = default;
  explicit Printer(Config cfg) : config{cfg} {}

  template <typename... Args>
  void printf(std::string_view fmt, Args&&... args) {
    print(std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
  }

  template <typename... Ts>
  void println(Ts... words) {
    print_indent();
    (print(words), ...);
    print(endl);
  }

  void print(Marker marker) {
    switch (marker) {
      case endl:
        StringBuffer::write(config.newline);
        cursor = config.offset;
        return;
      case separator:
        print(", ");
        return;
    }
    LIBREPR_UNREACHABLE();
  }

  void print(char character) {
    print_indent();
    if (cursor + 1 > config.max_width) {
      StringBuffer::write('\\');
      print(endl);

      // print previous indent - aligning to the last character doesn't make much sense
      print_indent();
    }
    ++cursor;
    StringBuffer::write(character);
  }

  void print(std::string_view word) {
    print_indent();
    if (word.size() > (config.max_width - cursor)) {
      const auto current_indent = cursor;
      // -1 for the \ continuation character
      // this means the line will be config.max_width + 1 long if you count the newline
      const auto max_remaining = (config.max_width - current_indent) - 1U;

      for (auto idx = 0U; idx < word.size(); idx += max_remaining) {
        if (idx != 0U) {
          StringBuffer::write('\\');
          print(endl);
          StringBuffer::write(' ', current_indent);
          cursor = current_indent;
        }
        const auto chunk = word.substr(idx, max_remaining);
        StringBuffer::write(chunk);
        cursor += chunk.size();
      }
      return;
    } else {
      cursor += word.size();
      StringBuffer::write(word);
    }
  }

  template <BlockKind kind, typename Block>
  void print_block(Block block) {
    switch (kind) {
      case parens:
        print('(');
        break;
      case braces:
        if (config.break_before_curly) {
          print(endl);
        }
        print('{');
        if (config.break_after_curly) {
          print(endl);
        }
        break;
      case brackets:
        print('[');
        break;
      case angles:
        print('<');
        break;
    }
    indent_level++;
    block();

    indent_level--;
    switch (kind) {
      case parens:
        print(')');
        break;
      case braces:
        print('}');
        break;
      case brackets:
        print(']');
        break;
      case angles:
        print('>');
        break;
    }
  }

  friend Printer& operator<<(Printer& self, std::string_view str) {
    self.print(str);
    return self;
  }

  friend Printer& operator<<(Printer& self, char character) {
    self.print(character);
    return self;
  }

  friend Printer& operator<<(Printer& self, Marker marker) {
    self.print(marker);
    return self;
  }

  // friend std::ostream& operator<<(std::ostream& stream, Printer& self) { return stream << self.buffer; }

  std::uint16_t indent_level = 0;

private:
  void print_indent() {
    if (cursor != config.offset) {
      // not on a new line, don't print indentation
      return;
    }
    const std::uint16_t indent_amount = config.offset + config.indent_amount * indent_level;
    StringBuffer::write(' ', indent_amount);
    cursor = indent_amount;
  }
  Config config{};
  std::uint16_t cursor = config.offset;
};

}  // namespace librepr::util
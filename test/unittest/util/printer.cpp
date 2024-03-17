#include <string>
#include <librepr/util/string/printer.h>
#include <gtest/gtest.h>

namespace librepr::util::test {

TEST(PrinterTest, wrap) {
  auto printer = Printer({.max_width = 10});
  printer.print("123456789123456789");
  EXPECT_STREQ(printer.extract().c_str(), "123456789\\\n123456789");

  printer = Printer({.max_width = 10, .offset = 5});
  printer.print("123456789");
  EXPECT_STREQ(printer.extract().c_str(), "     1234\\\n     5678\\\n     9");

  printer              = Printer({.max_width = 10, .indent_amount = 2});
  printer.indent_level = 2;
  printer.print("123456789");
  EXPECT_STREQ(printer.extract().c_str(), "    12345\\\n    6789");
}

TEST(PrinterTest, block) {
  auto printer = Printer();

  printer.print_block<Printer::angles>([]{});
  printer.print_block<Printer::brackets>([]{});
  printer.print_block<Printer::parens>([]{});
  printer.print_block<Printer::braces>([]{});
  EXPECT_STREQ(printer.extract().c_str(), "<>[](){\n}");

  printer = Printer({.indent_amount=2});
  printer.print_block<Printer::braces>([&] {
    printer.print_block<Printer::angles>([&] {
      printer.print_block<Printer::brackets>([&] {
        printer.print_block<Printer::parens>([] {});
      });
      printer.print(Printer::endl);
    });
  });
  EXPECT_STREQ(printer.extract().c_str(), "{\n  <[()]\n  >}");
}
}  // namespace librepr::util::test
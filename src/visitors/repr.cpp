#include <librepr/visitors/repr.h>

namespace librepr {
  void ReprVisitor::print_indent(){
    if (options.indent != 0) {
      result.append("\n");
      result.append(std::string(options.indent * level, ' '));
    }
  }

  void ReprVisitor::print_separator() {
    if (separate) {
      result.append(", ");
    } else {
      separate = true;
    }
    print_indent();
  }

  void ReprVisitor::increase_nesting() {
    separate = false;
    result += '{';
    ++level;
  }

  void ReprVisitor::decrease_nesting() {
    --level;
    print_indent();
    result += '}';
  }
}
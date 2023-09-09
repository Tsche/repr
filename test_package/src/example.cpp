#include "repr.h"
#include <vector>
#include <string>

int main() {
    repr();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    repr_print_vector(vec);
}

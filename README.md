# repr: C++ object and type serialization

[TOC]

librepr is a C++ static reflection and serialization library that provides a convenient way to generate string representations of C++ objects that can be used to reconstruct those objects. It is inspired by Python's `repr` function and is primarily designed to assist developers in debugging and logging.

## Features

- Generate string representations of C++ objects for reconstruction
- Customizable formatting for user-defined types
- Support for standard C++ data types and containers
- Rich type and object visitation

## Installation

### Using submodules

Navigate to your project directory in the terminal and run the following command to add repr as a submodule:
```bash
git submodule add https://github.com/Tsche/repr external/repr
```
Note that the path `external/repr` is chosen arbitrarily. You can add the submodule at any path you like.

You may need to adjust your include path. To do so add
```cmake
target_include_directories(your_target PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/repr/include)
```
to your CMakeLists.txt (replace `your_target` with the name of your target).

Alternatively if you do not use CMake simply pass your compiler an additional `-Iexternal/repr/include` (clang, gcc) or `/Iexternal/repr/include` (msvc) argument.

### Single header

You can download a single-header distribution of repr from the URL provided:
```bash
wget https://repr.palliate.io/amalgamated.h -O repr.h
```

This command downloads the single-header file and saves it as `repr.h` to the current working directory. You can place this file wherever convenient (ie `external/repr.h`).


### Compiler Explorer

[Compiler Explorer](https://godbolt.org) supports including headers from URLs directly, hence you can simply do
```cpp



//* -std=c++23
#include <https://repr.palliate.io/amalgamated.h>
#include <print>

int main(){
  int foo[3] = {1, 2, 3};
  std::print("foo: {}", repr(foo));
}



```

### Conan

> [!IMPORTANT]
> repr is not yet in Conan's package repository. The following will only work if you have the package installed locally.

You can include librepr in your C++ project using the [Conan](https://conan.io/) package manager:

1. Add librepr as a dependency in your `conanfile.txt`:

   ```
   [requires]
   repr/0.1
   ```

   Replace `0.1` with the desired version of librepr.

2. Install the dependencies using Conan:

   ```bash
   conan install .
   ```

3. In your CMakeLists.txt, link against librepr:

   ```cmake
   find_package(repr REQUIRED)
   target_link_libraries(your_target PRIVATE repr::repr)
   ```

4. Include the `repr` header in your C++ source files:

   ```cpp
   #include <repr>
   ```

You now have added librepr as a dependency to your project using Conan.


## Usage

The following example showcases usage of the `repr` function (or well, function object to be pedantic), which generates a reconstructible string representation of the given object. Furthermore we'll use `librepr::code_for` to also print the type. 

```cpp
//* gsnapshot -std=c++23 -O3
#include <repr>
#include <print>
#include <string>

struct Foo {
  unsigned number;
  struct Bar {
    char c;
    float f;
  } bar;
};

int main() {
    auto obj = Foo{.number=42U, .bar={.c='C', .f=1.0F}};
    // dump object
    std::println("Repr: {}", repr(obj));  // Repr: Foo{.number=42U, .bar={.c='C', .f=1.0F}};

    // dump type
    std::println("Type:\n{}", librepr::code_for<Foo>());

    // showcase difference between stringification and repr
    std::println("std::format = {}   repr = {}", obj.number, repr(obj.number));
}
```

## License

librepr is provided under the [MIT License](LICENSE). Feel free to use and modify it in your projects.


## Contributing

If you'd like to contribute to librepr, please fork the repository, make your changes, and submit a pull request. We welcome any contributions, including bug fixes, new features, and improvements. Bug reports submitted as issues are also very welcome.


## Contact

For questions or issues related to this library, please [open an issue](https://github.com/tsche/repr/issues).

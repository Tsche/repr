# librepr: Reconstructable String Representations for C++ Objects

librepr is a C++ library that provides a convenient way to generate string representations of C++ objects that can be used to reconstruct those objects. It is inspired by Python's `repr` function and is designed to assist developers in debugging and logging.

## Features

- Generate string representations of C++ objects for reconstruction.
- Customizable formatting for user-defined types.
- Support for standard C++ data types and containers.

## Installation

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

librepr provides a single function (or well, function object to be pedantic), `repr`, which generates a string representation of a C++ object that could be used for reconstruction:

```cpp
#include <repr>

int main() {
    int number = 42;
    std::string repr_str = repr(number);
    std::print("Repr of number: {}\n", repr_str);
}
```

### Customizing Representation for User-Defined Types

You can customize the representation of user-defined types by overloading the `repr` function for your class or struct. To do this, define specializations of the `repr` template function for your type:

```cpp
#include <repr>
#include <format>

struct MyType {
    unsigned value;

    MyType(unsigned val) : value(val) {}

    // Custom representation for MyType
    std::string repr() const {
        return std::format("MyType({})", repr(value));
    }
};

int main() {
    auto myObject = MyType{42U};
    std::string repr_str = repr(myObject);
    std::print("Repr of myObject: {}\n", repr_str);
}
```

## License

librepr is provided under the [MIT License](LICENSE.md). Feel free to use and modify it in your projects.

## Contributing

If you'd like to contribute to librepr, please fork the repository, make your changes, and submit a pull request. We welcome any contributions, including bug fixes, new features, and improvements. Bug reports submitted as issues are also very welcome.

## Contact

For questions or issues related to this library, please [open an issue](https://github.com/tsche/repr/issues).

Happy coding!

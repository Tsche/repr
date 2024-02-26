from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import copy
from conan.tools.cmake import CMakeToolchain


class ReprRecipe(ConanFile):
    name = "repr"
    version = "0.1"
    package_type = "header-library"

    # Optional metadata
    license = "MIT"
    author = "Tsche che@palliate.io"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of repr package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "benchmark": [True, False],
        "sanitizers": [True, False],
        "coverage": [True, False],
        "magic_enum": [True, False],
        "fmt": [True, False],
    }
    default_options = {"benchmark": False, "sanitizers": False,
                       "coverage": False, "magic_enum": False, "fmt": False}
    generators = "CMakeDeps"

    exports_sources = "CMakeLists.txt", "include/*"

    def needs_fmt(self):
        compiler = self.settings.compiler
        compiler_version = int(self.settings.get_safe("compiler.version") or "0".split('.', 1)[0])
        libcxx = self.settings.get_safe("compiler.libcxx")

        if compiler == "clang":
            libcxx = libcxx or "libstdc++" if self.settings.os == "Linux" else "libc++"

            if compiler_version < 17 and libcxx == "libc++":
                return True

        elif compiler == "gcc" and compiler_version < 13:
            return True

        return False

    def requirements(self):
        if self.options.magic_enum:
            self.requires("magic_enum/0.9.3", transitive_headers=True)

        if self.options.fmt or self.needs_fmt():
            self.requires("fmt/10.2.1",
                          transitive_headers=True,
                          transitive_libs=True)

        if self.options.benchmark:
            self.requires("benchmark/1.8.3")

        self.test_requires("gtest/1.14.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["ENABLE_BENCHMARK"]  = bool(self.options.benchmark)
        tc.variables["ENABLE_SANITIZERS"] = bool(self.options.sanitizers)
        tc.variables["ENABLE_COVERAGE"]   = bool(self.options.coverage)
        tc.variables["ENABLE_MAGIC_ENUM"] = bool(self.options.magic_enum)
        tc.variables["ENABLE_FMTLIB"]     = bool(self.options.fmt) or self.needs_fmt()

        tc.generate()

    def build(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake = CMake(self)
            cmake.configure()
            cmake.build()

    def package(self):
        copy(self, "repr", self.source_folder, self.package_folder)
        copy(self, "*.h", self.source_folder, self.package_folder)

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []

    def package_id(self):
        self.info.clear()

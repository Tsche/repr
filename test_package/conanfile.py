import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run


class ReprTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        lib_options = self.dependencies[self.tested_reference_str].options

        cmake = CMake(self)
        cmake.configure(
            variables={
                "ENABLE_MAGIC_ENUM": lib_options.magic_enum,
                "ENABLE_FMTLIB": lib_options.fmt,
            }
        )
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "example")
            self.run(cmd, env="conanrun")

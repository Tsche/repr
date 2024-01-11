from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy


class ReprRecipe(ConanFile):
    name = "repr"
    version = "0.1"
    package_type = "header-library"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of repr package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"sanitizers": [True, False], "coverage": [True, False]}
    default_options = {"sanitizers": False, "coverage": False}
    generators = "CMakeToolchain", "CMakeDeps"

    exports_sources = "CMakeLists.txt", "include/*"


    def requirements(self):
        self.requires("magic_enum/0.9.3", transitive_headers=True)
        self.test_requires("gtest/1.14.0")

    def layout(self):
        cmake_layout(self)

    def build(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake = CMake(self)
            cmake.configure(variables={'ENABLE_SANITIZERS': self.options.sanitizers, 
                                       'ENABLE_COVERAGE': self.options.coverage})
            cmake.build()

    def package(self):
        # TODO copy <repr>
        copy(self, "*.h", self.source_folder, self.package_folder)

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []

    def package_id(self):
        self.info.clear()

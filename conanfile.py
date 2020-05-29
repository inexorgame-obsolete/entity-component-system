from conans import CMake
from conans import ConanFile


class InexorECSConan(ConanFile):
    name = "inexor-ecs"
    version = "0.1.0"
    homepage = "https://github.com/inexorgame/entity-component-system"
    description = "A thread safe C++17 entity component system. "
    topics = ("entity", "component", "system", "inexor")
    url = "https://github.com/inexorgame/entity-component-system"
    settings = "os", "compiler", "arch", "build_type"
    generators = "cmake"
    no_copy_source = True
    license = "MIT"
    requires = (
        "spdlog/1.5.0",
    )
    options = {
        "build_benchmarks": [True, False],
    }
    default_options = {
        "build_benchmarks": False,
    }

    exports_sources = "*"

    def requirements(self):
        if self.options.build_benchmarks:
            self.requires("benchmark/1.5.0")

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy(pattern="LICENSE.md", src="./", dst="licenses")
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["inexor/ecs"]

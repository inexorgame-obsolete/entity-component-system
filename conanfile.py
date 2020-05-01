from conans import ConanFile, CMake

class InexorConan(ConanFile):

    settings = (
        "os",
        "compiler",
        "build_type",
        "arch"
    )

    requires = (
        "benchmark/1.5.0",
        "gtest/1.10.0",
        "spdlog/1.5.0",
    )

    generators = "cmake"

    def imports(self):
        # Copies all dll files from packages bin folder to my "bin" folder (win)
        self.copy("*.dll", dst="bin", src="bin")
        # Copies all dylib files from packages lib folder to my "lib" folder (macosx)
        self.copy("*.dylib*", dst="lib", src="lib") # From lib to lib
        # Copies all so files from packages lib folder to my "lib" folder (linux)
        self.copy("*.so*", dst="lib", src="lib") # From lib to lib

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

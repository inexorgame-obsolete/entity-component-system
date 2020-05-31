cmake_minimum_required(VERSION 3.7)
project(inexor-ecs)

# Stop in source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(INEXOR_ECS_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(INEXOR_ECS_BUILD_EXAMPLE "Build example" OFF)
option(INEXOR_ECS_BUILD_TESTS "Build tests" OFF)

message(STATUS "INEXOR_ECS_BUILD_BENCHMARKS = ${INEXOR_ECS_BUILD_BENCHMARKS}")
message(STATUS "INEXOR_ECS_BUILD_EXAMPLE = ${INEXOR_ECS_BUILD_EXAMPLE}")
message(STATUS "INEXOR_ECS_BUILD_TESTS = ${INEXOR_ECS_BUILD_TESTS}")

# Dependency setup with conan
include(conan_setup)

# Build ecs lib
add_subdirectory(src)

if(INEXOR_ECS_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(INEXOR_ECS_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if(INEXOR_ECS_BUILD_TESTS)
    add_subdirectory(tests)
endif()

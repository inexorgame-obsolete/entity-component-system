cmake_minimum_required(VERSION 3.7)
project(inexor-entity-component-system)

# Stop in source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

option(INEXOR_ECS_BUILD_EXAMPLE "Build example" ON)

message(STATUS "INEXOR_ECS_BUILD_EXAMPLE = ${INEXOR_ECS_BUILD_EXAMPLE}")

# Build ecs lib
add_subdirectory(src)

if(INEXOR_ECS_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

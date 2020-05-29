cmake_minimum_required(VERSION 3.7)

add_library(inexor-ecs ecs/thread_pool.cpp)

set_target_properties(
    inexor-ecs PROPERTIES

    CXX_EXTENSIONS OFF
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(MSVC)
    target_compile_options(inexor-ecs PRIVATE "/MP")
endif()

target_include_directories(
    inexor-ecs

    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(inexor-ecs PUBLIC ${CONAN_LIBS})

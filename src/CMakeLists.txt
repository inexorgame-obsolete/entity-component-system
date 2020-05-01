add_library(inexor-entity-component-system ecs/thread_pool.cpp)

set_target_properties(
    inexor-entity-component-system PROPERTIES

    CXX_EXTENSIONS OFF
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(MSVC)
    target_compile_options(inexor-entity-component-system PRIVATE "/MP")
endif()

target_include_directories(
    inexor-entity-component-system

    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(inexor-entity-component-system PUBLIC ${CONAN_LIBS})

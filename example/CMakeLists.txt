add_executable(inexor-ecs-example main.cpp)

set_target_properties(
    inexor-ecs-example PROPERTIES

    CXX_EXTENSIONS OFF
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(MSVC)
    target_compile_options(inexor-ecs-example PRIVATE "/MP")
    set_property(
        TARGET inexor-ecs-example
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
endif()

target_link_libraries(
    inexor-ecs-example

    PRIVATE
    inexor-ecs
)

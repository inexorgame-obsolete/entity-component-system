add_executable(inexor-entity-component-system-example main.cpp)

set_target_properties(
    inexor-entity-component-system-example PROPERTIES

    CXX_EXTENSIONS OFF
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(MSVC)
    target_compile_options(inexor-entity-component-system-example PRIVATE "/MP")
    set_property(
        TARGET inexor-entity-component-system-example
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
endif()

target_link_libraries(
    inexor-entity-component-system-example

    PRIVATE
    inexor-entity-component-system
)

install(
    TARGETS inexor-ecs
    EXPORT inexor-ecsTargets
    LIBRARY DESTINATION lib/
    ARCHIVE DESTINATION lib/
    RUNTIME DESTINATION bin/
    PUBLIC_HEADER DESTINATION include/inexor/ecs/
)
# headers (copies all headers)
FILE(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/include/inexor/ecs/*.h" "${CMAKE_CURRENT_BINARY_DIR}/include/inexor/ecs/*.h")
INSTALL(FILES ${headers} DESTINATION include/inexor/ecs/)

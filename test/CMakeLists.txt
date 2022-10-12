set(target test)

add_executable(${target} main.c)
target_link_libraries(${target} ${PROJECT_NAME})
target_include_directories(${target} PRIVATE ${PROJECT_NAME})

target_compile_definitions(${target} PRIVATE
    "-Dtest_path=\"${CMAKE_CURRENT_SOURCE_DIR}\""
)
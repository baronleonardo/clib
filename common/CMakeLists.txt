add_library(common
    INTERFACE
)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(common PROPERTIES LINKER_LANGUAGE C)


# test
if(test)
    add_executable(test_common test_common.c)
    target_link_libraries(test_common common)
    add_test(NAME test_common COMMAND test_common)
endif()
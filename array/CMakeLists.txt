add_library(array
    array.h
    array.c
    array_internal.h
)
target_include_directories(array PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# install
set_target_properties(array
    PROPERTIES
    PUBLIC_HEADER array.h
)
install(TARGETS array)


# test
if(test)
    add_executable(test_array test_array.c)
    target_link_libraries(test_array array)
    add_test(NAME test_array COMMAND test_array)
endif()
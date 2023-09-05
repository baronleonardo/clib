add_library(str
    str.h
    str.c
    str_internal.h
)
target_include_directories(str PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(str PUBLIC common)


# install
set_target_properties(str
    PROPERTIES
    PUBLIC_HEADER str.h
)
install(TARGETS str)


# test
if(test)
    add_executable(test_str test_str.c)
    target_link_libraries(test_str str)
    add_test(NAME test_str COMMAND test_str)
endif()
add_library(io
    io.h
    io.c
    io_internal.h
)
target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(io PUBLIC common)


# install
set_target_properties(io
    PROPERTIES
    PUBLIC_HEADER io.h
)
install(TARGETS io)


# test
if(test)
    add_executable(test_io test_io.c)
    target_link_libraries(test_io io)
    target_compile_definitions(test_io PUBLIC test_assets="${CMAKE_CURRENT_SOURCE_DIR}/test_assets")
    add_test(NAME test_io COMMAND test_io)
endif()
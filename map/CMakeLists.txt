add_library(map
    map.h
    map.c
    map_internal.h
)
target_include_directories(map PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(map PUBLIC common)


# install
set_target_properties(map
    PROPERTIES
    PUBLIC_HEADER map.h
)
install(TARGETS map)


# test
if(test)
    add_executable(test_map test_map.c)
    target_link_libraries(test_map map)
    add_test(NAME test_map COMMAND test_map)
endif()
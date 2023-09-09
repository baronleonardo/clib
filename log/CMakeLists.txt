add_library(log
    log.h
    log.c
    log_internal.h
)
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(log PUBLIC common)


# install
set_target_properties(log
    PROPERTIES
    PUBLIC_HEADER log.h
)
install(TARGETS log)


# test
if(test)
    add_executable(test_log test_log.c)
    target_link_libraries(test_log log)
    add_test(NAME test_log COMMAND test_log)
endif()
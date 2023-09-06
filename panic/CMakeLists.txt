add_library(panic
    INTERFACE
)
target_include_directories(panic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(panic PROPERTIES LINKER_LANGUAGE C)


# install
set_target_properties(panic
    PROPERTIES
    PUBLIC_HEADER panic.h
)
install(TARGETS panic)
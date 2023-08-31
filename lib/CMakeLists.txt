set(target ${PROJECT_NAME})

add_library(${target}
    clib.c
    string.c
    log.c
    error.c
    unittest.c
    list.c
    file.c
    print.c
    allocator.c
)

target_include_directories(${target}
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
)
set(target lib)

add_library(${target}
    clib.c
    string.c
    log.c
    error.c
    unittest.c
)

target_include_directories(${target}
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
)
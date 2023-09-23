cmake_minimum_required(VERSION 3.21)
project(clib C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # PIC
set(CMAKE_DEBUG_POSTFIX _debug)
if (MSVC)
    # warning level 4
    add_compile_options(
        /Wall
        /WX
        /utf-8
        $<$<CONFIG:DEBUG>:/fsanitize=address>
        $<$<CONFIG:DEBUG>:/RTCcsu>
    )
else()
    # additional warnings
    add_compile_options(
        -Wall
        -Wpedantic
        -Werror
        $<$<CONFIG:RELEASE>:-D_FORTIFY_SOURCE=2>
        $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>
        $<$<CONFIG:DEBUG>:-fstack-protector-strong>
        $<$<CONFIG:DEBUG>:--param=ssp-buffer-size=4>
        $<$<CONFIG:DEBUG>:-fstack-clash-protection>
        $<$<CONFIG:DEBUG>:-fcf-protection>
    )
    add_link_options($<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

option(BUILD_SHARED_LIBS "Build libraries as SHARED" OFF)   # static by default
option(ui "enable ui" ON)
option(test "enable tests" OFF)
if(test)
    enable_testing()
endif()

add_subdirectory(array)
add_subdirectory(common)
add_subdirectory(io)
add_subdirectory(log)
add_subdirectory(map)
add_subdirectory(str)
if(ui)
add_subdirectory(ui)
endif()
cmake_minimum_required(VERSION 3.21)
project(clib C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # PIC
if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wpedantic -Wno-flexible-array-extensions)
endif()

option(BUILD_SHARED_LIBS "Build libraries as SHARED" OFF)   # static by default
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
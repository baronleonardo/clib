option(gtk "enable gtk backend" OFF)
option(qt "enable qt backend" OFF)
option(windows_ui "enable windows ui backend" OFF)
option(mac_ui "enable mac ui backend" OFF)
option(android_ui "enable android ui backend" OFF)
option(ios_ui "enable ios ui backend" OFF)

add_library(ui
    ui.h
    ui.c
    ui_internal.h
    ui_window.h
    ui_window.c
    # ui_button.h
    # ui_button.c
)
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ui PUBLIC common)

if(NOT (gtk AND qt AND windows_ui AND mac_ui AND android_ui AND ios_ui))
    message("== Warning: no ui backend registerd")
    message("== Info: choose based on the current system")
    if(UNIX AND NOT APPLE)
        set(gtk ON)
        message("== Option: gtk ON")
    elseif(WIN32)
        set(windows_ui ON)
        message("== Option: windows_ui ON")
    elseif(APPLE)
        set(mac_ui ON)
        message("== Option: mac_ui ON")
    elseif(ANDROID)
        set(android_ui ON)
        message("== Option: android_ui ON")
    elseif(IOS)
        set(ios_ui ON)
        message("== Option: ios_ui ON")
    endif()
endif()


# backends
if(gtk)
    target_compile_definitions(ui PRIVATE gtk=1)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    target_link_libraries(ui PUBLIC ${GTK_LIBRARIES})
    target_include_directories(ui PUBLIC ${GTK_INCLUDE_DIRS})
elseif(qt)
    target_compile_definitions(ui PRIVATE qt=1)
elseif(windows_ui)
    target_compile_definitions(ui PRIVATE windows_ui=1)
elseif(mac_ui)
    target_compile_definitions(ui PRIVATE mac_ui=1)
elseif(android_ui)
    target_compile_definitions(ui PRIVATE android_ui=1)
elseif(ios_ui)
    target_compile_definitions(ui PRIVATE ios_ui=1)
endif()


# install
set_target_properties(ui
    PROPERTIES
    PUBLIC_HEADER "ui.h;ui_button.h"
)
install(TARGETS ui)


# test
if(test)
    add_executable(test_ui test_ui.c)
    target_link_libraries(test_ui ui)
    add_test(NAME test_ui COMMAND test_ui)
endif()
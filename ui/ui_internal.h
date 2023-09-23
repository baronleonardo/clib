#ifndef UI_INTERNAL_H
#define UI_INTERNAL_H

#include <ui.h>
#include <ui_window.h>
#include <cassert.h>

typedef struct UiChildEvent {
    void(*handler)(UiWidget*, void*);
    void* extra_data;
} UiChildEvent;

#define ERROR_MSG_NOT_ACTIVATED(function_name) \
    "Error: " function_name " should be called from 'construction_handler' of 'ui_main_loop.'"

#ifdef WIN32
#include <windows.h>

static inline BOOL CALLBACK
ui_internal_event_window_destroy(HWND hwnd, LPARAM lParam) {
    (void)hwnd;
    (void)lParam;

    UiChildEvent* child_event = (UiChildEvent*)GetWindowLongPtr((HWND)lParam, GWL_USERDATA);
    if(child_event) free(child_event);

    return true;
}
#endif // WIN32

#endif // UI_INTERNAL_H

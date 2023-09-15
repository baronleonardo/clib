#ifndef UI_INTERNAL_WIN_H
#define UI_INTERNAL_WIN_H

#include <stdbool.h>
#include <stddef.h>
#include <windows.h>

struct UiBackend {
    bool is_activated;
    const char* title;
    size_t title_len;
    WNDCLASSEX backend;
    void (*on_activate_handler)(struct UiBackend*);
};
typedef struct UiBackend* UiBackend;
typedef void* UiBackendWidget;

UiBackend
ui_internal_win_create(const char* title, size_t title_len);


void
ui_internal_win_child_add(UiBackend self, UiBackendWidget* widget);


void
ui_internal_win_mainloop(UiBackend self, void on_activate_handler(UiBackend self));


void
ui_internal_win_destroy(UiBackend* self);

#endif // UI_INTERNAL_WIN_H

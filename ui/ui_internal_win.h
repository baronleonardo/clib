#ifndef UI_INTERNAL_WIN_H
#define UI_INTERNAL_WIN_H

#include <stdbool.h>
#include <stddef.h>

struct UiBackend {
    bool is_activated;
    const char* title;
    size_t title_len;
    void* backend;
};
typedef struct UiBackend* UiBackend;
typedef void* UiBackendButton;
typedef void* UiBackendWidget;

UiBackend
ui_internal_win_create(const char* title, size_t title_len);


void
ui_internal_win_child_add(UiBackend self, UiBackendWidget* widget);


UiBackendButton*
ui_internal_win_button_add(UiBackend self, const char* label, size_t label_len);


void
ui_internal_win_button_event_clicked(
    UiBackendButton* button,
    void on_click_event(UiBackendButton* button, void* extra_data),
    void* extra_data
);


void
ui_internal_win_mainloop(
    UiBackend self,
    char* argv[],
    int argc,
    void on_activate_handler(UiBackend self)
);


void
ui_internal_win_destroy(UiBackend* self);

#endif // UI_INTERNAL_WIN_H

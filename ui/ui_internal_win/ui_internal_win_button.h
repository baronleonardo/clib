#ifndef UI_INTERNAL_WIN_BUTTON_H
#define UI_INTERNAL_WIN_BUTTON_H

#include "ui_internal_win.h"

typedef void* UiBackendButton;

UiBackendButton*
ui_internal_win_button_add(UiBackend self, const char* label, size_t label_len);


void
ui_internal_win_button_event_clicked(
    UiBackendButton* button,
    void on_click_event(UiBackendButton* button, void* extra_data),
    void* extra_data
);

#endif // UI_INTERNAL_WIN_BUTTON_H
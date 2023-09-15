#ifndef UI_INTERNAL_GTK_BUTTON_H
#define UI_INTERNAL_GTK_BUTTON_H

#include <ui_internal_gtk/ui_internal_gtk.h>

UiBackendButton*
ui_internal_gtk_button_add(UiBackend self, const char* label, size_t label_len);


void
ui_internal_gtk_button_event_clicked(
    UiBackendButton* button,
    void on_click_event(UiBackendButton* button, void* extra_data),
    void* extra_data
);

#endif // UI_INTERNAL_GTK_BUTTON_H

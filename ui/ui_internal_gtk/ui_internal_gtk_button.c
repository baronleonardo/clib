#include "ui_internal_gtk_button.h"
#include <cassert.h>

UiBackendButton*
ui_internal_gtk_button_add(
    UiBackend self,
    const char* label,
    size_t label_len
) {
    GtkChild* button = gtk_button_new_with_label(label);
    cassert_always(button);

    return GTK_BUTTON(button);
}

void
ui_internal_gtk_button_event_clicked(
    UiBackendButton* button,
    void on_click_event(UiBackendButton* button, void* extra_data),
    void* extra_data
) {
    g_signal_connect(button, "clicked", G_CALLBACK(on_click_event), extra_data);
}

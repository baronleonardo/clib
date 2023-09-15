#include "ui_internal_gtk_button.h"
#include <cassert.h>

void
ui_internal_gtk_child_add(UiBackend self, UiBackendWidget* widget) {
    GtkWindow* current_window = gtk_application_get_active_window(self->backend);
    gtk_container_add(GTK_CONTAINER(current_window), widget);
}

UiBackendButton*
ui_internal_gtk_button_add(
    UiBackend self,
    const char* label,
    size_t label_len
) {
    GtkWidget* button = gtk_button_new_with_label(label);
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

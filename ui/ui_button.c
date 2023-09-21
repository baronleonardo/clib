#include <ui_button.h>
#include <cassert.h>

#ifdef gtk
#include <gtk/gtk.h>

UiButton*
ui_button_create(Ui* self, const char* label, size_t label_len) {
    cassert(self);
    cassert(label);
    cassert(label_len > 0);

    GtkButton* button = GTK_BUTTON(gtk_button_new_with_label(label));
    cassert_always(button);

    gtk_widget_show(GTK_WIDGET(button));

    return button;
}

void
ui_button_event_clicked(UiButton* button, void on_click_event(UiButton* button, void* extra_data), void* extra_data) {
    g_signal_connect(button, "clicked", G_CALLBACK(on_click_event), extra_data);
}
#endif // gtk


#ifdef windows_ui
#include <ui_internal_win/ui_internal_win_button.h>

UiButton*
ui_button_create(
    Ui* self,
    const char* label,
    size_t label_len
) {
    cassert(self);
    cassert(label);
    cassert(label_len > 0);

    
}

void
ui_button_event_clicked(UiButton* button, void on_click_event(UiButton* button, void* extra_data), void* extra_data) {
    ui_internal_win_button_event_clicked(button, (void (*)(UiButton*, void*))on_click_event, extra_data);
}
#endif // windows_ui
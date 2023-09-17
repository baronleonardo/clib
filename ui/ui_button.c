#include <ui_button.h>
#include <cassert.h>

#ifdef gtk
#include <ui_internal_gtk/ui_internal_gtk_button.h>

UiButton
ui_button_create(
    Ui self,
    const char* label,
    size_t label_len
) {
    cassert(self);
    cassert(label);
    cassert(label_len > 0);

    return (UiBackendButton *)ui_internal_gtk_button_add(self, label, label_len);
}

void
ui_button_event_clicked(UiButton button, void on_click_event(UiButton button, void* extra_data), void* extra_data) {
    ui_internal_gtk_button_event_clicked(button, (void (*)(UiBackendButton*, void*))on_click_event, extra_data);
}
#endif // gtk


#ifdef windows_ui
#include <ui_internal_win/ui_internal_win_button.h>

UiButton
ui_button_create(
    Ui self,
    const char* label,
    size_t label_len
) {
    cassert(self);
    cassert(label);
    cassert(label_len > 0);

    return (UiBackendButton *)ui_internal_win_button_add(self, label, label_len);
}

void
ui_button_event_clicked(UiButton button, void on_click_event(UiButton button, void* extra_data), void* extra_data) {
    ui_internal_win_button_event_clicked(button, (void (*)(UiBackendButton*, void*))on_click_event, extra_data);
}
#endif // windows_ui


#include <ui.h>
#include <cassert.h>

#ifdef gtk
#include <ui_internal_gtk.h>

Ui
ui_create(const char* title, size_t title_len) {
    cassert(title);
    cassert(title_len > 0);

    return ui_internal_gtk_create(title, title_len);
}

void
ui_child_add(Ui self, UiWidget widget) {
    cassert(self);
    cassert(widget);

    ui_internal_gtk_child_add(self, widget);
}

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

void
ui_mainloop(Ui self, char* argv[], int argc, void on_activate_handler(Ui self)) {
    ui_internal_gtk_mainloop(self, argv, argc, on_activate_handler);
}

void
ui_destroy(Ui* self) {
    cassert(self && *self);

    ui_internal_gtk_destroy(self);
}

#endif // gtk

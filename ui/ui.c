

#include <ui.h>
#include <cassert.h>

#ifdef gtk
#include <ui_internal_gtk/ui_internal_gtk.h>

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

void
ui_mainloop(Ui self, void on_activate_handler(Ui self)) {
    ui_internal_gtk_mainloop(self, on_activate_handler);
}

void
ui_destroy(Ui* self) {
    cassert(self && *self);

    ui_internal_gtk_destroy(self);
}

#endif // gtk

#ifdef windows_ui
#include <ui_internal_win/ui_internal_win.h>

Ui
ui_create(const char* title, size_t title_len) {
    return ui_internal_win_create(title, title_len);
}

void
ui_child_add(Ui self, UiWidget widget) {

}

void
ui_mainloop(Ui self, void on_activate_handler(Ui self)) {
    ui_internal_win_mainloop(self, on_activate_handler);
}

void
ui_destroy(Ui* self) {
    ui_internal_win_destroy(self);
}
#endif // windows_ui

#ifndef UI_INTERNAL_GTK_H
#define UI_INTERNAL_GTK_H

#include <gtk/gtk.h>

#include <stdbool.h>

struct UiBackend {
    bool is_activated;
    const char* title;
    size_t title_len;
    GtkApplication* backend;
};
typedef struct UiBackend* UiBackend;
typedef GtkButton UiBackendButton;
typedef GtkWidget UiBackendWidget;


UiBackend
ui_internal_gtk_create(const char* title, size_t title_len);


void
ui_internal_gtk_child_add(UiBackend self, UiBackendWidget* widget);


void
ui_internal_gtk_mainloop(UiBackend self, void on_activate_handler(UiBackend self));


void
ui_internal_gtk_destroy(UiBackend* self);


#endif // UI_INTERNAL_GTK_H

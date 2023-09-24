#include <ui_box.h>
#include <cassert.h>

#ifdef gtk
#include <gtk/gtk.h>

UiBox*
ui_box_create(UiWidget* parent, bool is_vertical, size_t spacing) {
    GtkBox* self = (GtkBox*)gtk_box_new(
        (is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL),
        spacing
    );
    cassert(self);

    gtk_container_add(GTK_CONTAINER(parent), GTK_WIDGET(self));
    gtk_widget_show(GTK_WIDGET(self));

    return (void*)self;
}

// void
// ui_box_push(UiBox* self, UiWidget* widget) {
//     cassert(self);
//     cassert(widget);

//     gtk_box_pack_start(GTK_BOX(self), GTK_WIDGET(widget), false, true, 0);
// }

void
ui_box_remove(UiBox* self, UiWidget* widget) {
    cassert(self);
    cassert(widget);

    gtk_container_remove(GTK_CONTAINER(self), GTK_WIDGET(widget));
}

void
ui_box_destroy(UiBox** self) {
    cassert(self && *self);

    g_object_unref(*self);
    *self = NULL;
}
#endif // gtk

#ifdef windows_ui
#endif // windows_ui
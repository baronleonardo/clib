#include <ui_internal_gtk.h>
#include <cassert.h>

typedef struct {
    void (*on_activate_handler)(UiBackend self);
    UiBackend backend_ui;    
} UiActivationCallBackExtraData;

static void
ui_internal_gtk_on_activate_handler(GtkApplication* self, void* extra_data);


UiBackend
ui_internal_gtk_create(const char* title, size_t title_len) {
    GtkApplication* gtk_app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
    cassert_always(gtk_app);

    UiBackend app = malloc(sizeof(struct UiBackend));
    cassert(app);
    app->is_activated = false;
    app->backend = gtk_app;
    app->title = title;
    app->title_len = title_len;

    return app;
}

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

void
ui_internal_gtk_mainloop(
    UiBackend self,
    char* argv[],
    int argc,
    void on_activate_handler(UiBackend self)
){
    UiActivationCallBackExtraData extra_data = {
        .backend_ui = self,
        .on_activate_handler = on_activate_handler
    };

    g_signal_connect (self->backend, "activate", G_CALLBACK(ui_internal_gtk_on_activate_handler), &extra_data);

    extra_data.backend_ui->is_activated = true;
    cassert(g_application_run(G_APPLICATION(self->backend), argc, argv) == 0);
}

void
ui_internal_gtk_destroy(UiBackend* self) {
    g_object_unref((*self)->backend);
    free(*self);
    *self = NULL;
}

void
ui_internal_gtk_on_activate_handler(GtkApplication* self, void* extra_data) {
    UiBackend backend = ((UiActivationCallBackExtraData*)extra_data)->backend_ui;

    // create a window
    GtkWidget* gtk_window = gtk_application_window_new(self);
    cassert_always(gtk_window);
    gtk_window_set_title(GTK_WINDOW(gtk_window), backend->title);

    ((UiActivationCallBackExtraData*)extra_data)->on_activate_handler(backend);

    gtk_widget_show_all(GTK_WIDGET(gtk_window));
}

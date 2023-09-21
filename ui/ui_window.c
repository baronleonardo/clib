#include <cassert.h>
#include <ui_window.h>
#include <ui_internal.h>

#ifdef gtk
#include <gtk/gtk.h>

UiWindow*
ui_window_add(Ui* self, const char* title, size_t title_len, size_t width, size_t height) {
    cassert(title);
    cassert(title_len > 0);
    cassert(width > 0);
    cassert(height > 0);
    cassert_always_msg(self->is_activated, ERROR_MSG_NOT_ACTIVATED("ui_window_add"));

    // create a window
    GtkWindow* gtk_window = GTK_WINDOW(gtk_application_window_new(self->backend));
    cassert_always(gtk_window);
    gtk_window_set_title(gtk_window, title);
    gtk_window_set_default_size (gtk_window, width, height);

    gtk_widget_show(GTK_WIDGET(gtk_window));

    return gtk_window;
}

void
ui_window_child_add(UiWindow* window, UiChild* child) {
    cassert(window);
    cassert(child);

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(child));
}

void
ui_window_show(UiWindow* window, bool is_shown) {
    if(is_shown) {
        gtk_widget_show_all(GTK_WIDGET(window));
    } else {
        gtk_widget_hide(GTK_WIDGET(window));
    }
}
#endif // gtk


#ifdef windows_ui
#include <windows.h>

UiWindow*
ui_window_add(Ui* self, const char* title, size_t title_len, size_t width, size_t height) {
    cassert(self);
    cassert(title);
    cassert(title_len > 0);
    cassert(title[title_len] == '\0');
    cassert_always_msg(self->is_activated, ERROR_MSG_NOT_ACTIVATED("ui_window_add"));

    HWND window = CreateWindowA(
        ((WNDCLASSEX*)self->backend)->lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        ((WNDCLASSEX*)self->backend)->hInstance,
        self
    );
    cassert_always(window);

    STARTUPINFO startup_info;
    /// Specifies the window station, desktop, standard handles, and appearance of the main window for a process at creation time.
    GetStartupInfo(&startup_info);  
    int nCmdShow = startup_info.wShowWindow;

    cassert_always(ShowWindow(window, nCmdShow));
    cassert_always(UpdateWindow(window));

    return window;
}

void
ui_window_show(UiWindow* window, bool is_shown) {
    cassert(window);
    
    if(is_shown) {
        // STARTUPINFO startup_info;
        // /// Specifies the window station, desktop, standard handles, and appearance of the main window for a process at creation time.
        // GetStartupInfo(&startup_info);  
        // int nCmdShow = startup_info.wShowWindow;

        cassert_always(ShowWindow((HWND)window, SW_SHOW));
    } else {
        cassert_always(ShowWindow((HWND)window, SW_HIDE));
    }
}
#endif // windows_ui

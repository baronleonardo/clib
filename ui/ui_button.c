#include <ui_button.h>
#include <cassert.h>

#ifdef gtk
#include <gtk/gtk.h>

UiButton*
ui_button_create(UiWidget* parent, const char* label, size_t label_len) {
    cassert(parent);
    cassert(label);
    cassert(label_len > 0);

    GtkButton* button = GTK_BUTTON(gtk_button_new_with_label(label));
    cassert_always(button);

    gtk_widget_set_parent(GTK_WIDGET(button), parent);
    gtk_widget_show(GTK_WIDGET(button));

    return button;
}

void
ui_button_event_clicked(UiButton* button, void on_click_event(UiButton* button, void* extra_data), void* extra_data) {
    g_signal_connect(button, "clicked", G_CALLBACK(on_click_event), extra_data);
}
#endif // gtk


#ifdef windows_ui
#include <windows.h>
#include <windowsx.h>
#include <ui_internal.h>

UiButton*
ui_button_create(
    UiWidget* parent,
    const char* label,
    size_t label_len
) {
    cassert(parent);
    cassert(label);
    cassert(label_len > 0);

    LONG units = GetDialogBaseUnits();
    HWND button = CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed 
        label,      // Button text 
        // WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
        CW_USEDEFAULT,         // x position 
        CW_USEDEFAULT,         // y position 
        MulDiv(LOWORD(units), 50, 4),        // Button width with default size
        MulDiv(HIWORD(units), 14, 8),        // Button height with default size
        (HWND)parent,     // Parent window
        NULL,     // No menu.
        (HINSTANCE)GetWindowLongPtr((HWND)parent, GWLP_HINSTANCE),
        NULL // Pointer not needed.
    );
    cassert_always(button);

    return button;
}

void
ui_button_event_clicked(
    UiButton* button,
    void on_click_event(UiButton* button, void* extra_data),
    void* extra_data
) {
    cassert(button);
    cassert(on_click_event);

    /// TODO: free me please
    UiChildEvent* child_event = (UiChildEvent*)malloc(sizeof(UiChildEvent));
    cassert(child_event);
    child_event->extra_data = extra_data;
    child_event->handler = on_click_event;

    SetLastError(0);
    SetWindowLongPtr((HWND)button, GWL_USERDATA, (LONG_PTR)child_event);
    cassert_always(GetLastError() == 0);
}

void
ui_button_destroy(UiButton** button) {
    cassert(button && *button);

    UiChildEvent* child_event = (UiChildEvent*)GetWindowLongPtr((HWND)(*button), GWL_USERDATA);
    free(child_event);
    cassert(DestroyWindow((HWND)(*button)));
}
#endif // windows_ui
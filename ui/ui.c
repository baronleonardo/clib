

#include <ui.h>
#include <ui_internal.h>
#include <cassert.h>

#ifdef gtk
#include <gtk/gtk.h>

typedef struct {
    void (*construction_handler)(Ui* self);
    Ui* backend;
} UiActivationCallBackExtraData;

static void
ui_internal_gtk_on_activate_handler(GtkApplication* self, void* extra_data);


Ui*
ui_create(const char* class_name, size_t class_name_len) {
    // cassert(class_name);
    // cassert(class_name_len > 0);
    cassert_always(!class_name || g_application_id_is_valid(class_name));

    GtkApplication* gtk_app = gtk_application_new(class_name, G_APPLICATION_FLAGS_NONE);
    cassert_always(gtk_app);

    Ui* app = (Ui*)malloc(sizeof(Ui));
    cassert(app);
    app->is_activated = false;
    app->backend = gtk_app;

    return app;
}

void
ui_mainloop(Ui* self, void construction_handler(Ui* self)) {
    cassert(self);
    cassert(construction_handler);

    self->callback = malloc(sizeof(UiActivationCallBackExtraData));
    cassert(self->callback);

    ((UiActivationCallBackExtraData*)self->callback)->backend = self;
    ((UiActivationCallBackExtraData*)self->callback)->construction_handler = construction_handler;

    g_signal_connect(self->backend, "activate", G_CALLBACK(ui_internal_gtk_on_activate_handler), self->callback);

    self->is_activated = true;
    cassert(g_application_run(G_APPLICATION(self->backend), 0, NULL) == 0);
}

void
ui_destroy(Ui** self) {
    cassert(self && *self);

    if((*self)->callback) free((*self)->callback);
    g_object_unref((*self)->backend);
    free(*self);
    *self = NULL;
}

void
ui_internal_gtk_on_activate_handler(GtkApplication* self, void* extra_data) {
    Ui* backend = ((UiActivationCallBackExtraData*)extra_data)->backend;

    // create a window
    // GtkWindow* gtk_window = GTK_WINDOW(gtk_application_window_new(self));
    // cassert_always(gtk_window);
    // gtk_window_set_title(GTK_WINDOW(gtk_window), backend->title);

    ((UiActivationCallBackExtraData*)extra_data)->construction_handler(backend);
}
#endif // gtk


#ifdef windows_ui
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdbool.h>

#include <cassert.h>

static void
ui_internal_win_on_activate_handler(void* self, void* extra_data);

static LRESULT __stdcall ui_internal_win_event_handler (
     HWND   hwnd    // Window Handle (Window object)
    ,UINT   msg     // Window Message  
    ,WPARAM wParam  // Additional message information
    ,LPARAM lParam  // Additional message information
);


Ui*
ui_create(const char* class_name, size_t class_name_len) {
    cassert(class_name);
    cassert(class_name_len > 0);

    Ui* app = (Ui*)calloc(1, sizeof(Ui));
    cassert(app);

    WNDCLASSEX* backend = (WNDCLASSEX*)calloc(1, sizeof(WNDCLASSEX));
    cassert(backend);
    
    /// TODO: this should be a unique class name
    ///       pass it as a parameter
    backend->lpszClassName = class_name;
    backend->lpfnWndProc = &ui_internal_win_event_handler;
    backend->cbSize = sizeof(WNDCLASSEX);
    // backend->style  = 0;
    // backend->cbClsExtra = 0;
    // backend->cbWndExtra = 0;
    backend->hInstance = GetModuleHandle(NULL);
    // backend->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    backend->hCursor = LoadCursor(NULL, IDC_ARROW);
    backend->hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    // backend->lpszMenuName = NULL;	
    // backend->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    app->backend = backend;
    cassert_always(RegisterClassEx(app->backend));

    return app;
}

void
ui_mainloop(Ui* self, void construction_handler(Ui* self)) {
    // STARTUPINFO startup_info;
    // /// Specifies the window station, desktop, standard handles, and appearance of the main window for a process at creation time.
    // GetStartupInfo(&startup_info);  
    // int nCmdShow = startup_info.wShowWindow;

    self->is_activated = true;
    construction_handler(self);

    // HWND window = CreateWindowA(
    //     self->backend.lpszClassName,
    //     "self->title",
    //     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    //     CW_USEDEFAULT,
    //     CW_USEDEFAULT,
    //     800,
    //     600,
    //     NULL,
    //     NULL,
    //     self->backend.hInstance,
    //     self
    // );
    // cassert_always(window);

    // cassert_always(ShowWindow(window, nCmdShow));
    // cassert_always(UpdateWindow(window));

    // Win32 message structure 
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void
ui_destroy(Ui** self) {
    cassert(self && *self);

    free((*self)->backend);
    free(*self);
    *self = NULL;
}

LRESULT __stdcall ui_internal_win_event_handler (
     HWND   hwnd    // Window Handle (Window object)
    ,UINT   msg     // Window Message  
    ,WPARAM wParam  // Additional message information
    ,LPARAM lParam  // Additional message information
) {
    // Process messages 
    switch(msg) {
        case WM_CREATE:
            // Ui* self = (Ui*)(((CREATESTRUCT*)lParam)->lpCreateParams);
            // self->on_activate_handler(self);
            break;
        case WM_CLOSE:		
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            EnumChildWindows(hwnd, ui_internal_event_window_destroy, 0);
            PostQuitMessage(0);
            break;
        case WM_MOVE:
            break;
        case WM_SIZE:
            break;
        case WM_COMMAND:
            switch(HIWORD(wParam)) {
                case BN_CLICKED:
                    UiChildEvent* child_event = (UiChildEvent*)GetWindowLongPtr((HWND)lParam, GWL_USERDATA);
                    child_event->handler((HWND)lParam, child_event->extra_data);
                    // puts("clicked");
                    break;
                case EN_UPDATE:
                    // char buf[2000];
                    // Edit_GetText((HWND)lParam, buf, 2000);
                    // puts(buf);
                    break;
                case CBN_SELCHANGE:
                    // char buf[2000];
                    // ComboBox_GetText((HWND)lParam, buf, 2000);
                    // puts(buf);
                    break;
                // default: 
                //     return DefWindowProc(hwnd, msg, wParam, lParam);
            }
            break;
        case WM_PAINT:
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
#endif // windows_ui

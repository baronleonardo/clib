

#include <ui.h>
#include <cassert.h>

#ifdef gtk
#include <ui_internal_gtk/ui_internal_gtk.h>

Ui
ui_create(const char* class_name, size_t class_name_len) {
    cassert(title);
    cassert(title_len > 0);

    return ui_internal_gtk_create(title, title_len);
}

void
ui_child_add(Ui self, UiChild child) {
    cassert(self);
    cassert(child);

    ui_internal_gtk_child_add(self, child);
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
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdbool.h>

#include <cassert.h>

typedef struct UiBackend {
    bool is_activated;
    const char* title;
    size_t title_len;
    WNDCLASSEX backend;
    void (*on_activate_handler)(struct UiBackend*);
} UiBackend;
typedef void* UiChild;

typedef struct {
    void (*on_activate_handler)(Ui self);
    UiBackend* backend;
} UiActivationCallBackExtraData;

static void
ui_internal_win_on_activate_handler(void* self, void* extra_data);

static LRESULT __stdcall ui_internal_win_event_handler (
     HWND   hwnd    // Window Handle (Window object)
    ,UINT   msg     // Window Message  
    ,WPARAM wParam  // Additional message information
    ,LPARAM lParam  // Additional message information
);


Ui
ui_create(const char* class_name, size_t class_name_len) {
    (void)class_name_len;

    UiBackend* app = (UiBackend*)calloc(1, sizeof(struct UiBackend));
    cassert(app);
    
    /// TODO: this should be a unique class name
    ///       pass it as a parameter
    app->backend.lpszClassName = class_name;
    app->backend.lpfnWndProc = &ui_internal_win_event_handler;
    app->backend.cbSize = sizeof(WNDCLASSEX);
    // app->backend.style  = 0;
    // app->backend.cbClsExtra = 0;
    // app->backend.cbWndExtra = 0;
    app->backend.hInstance = GetModuleHandle(NULL);
    // app->backend.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    app->backend.hCursor = LoadCursor(NULL, IDC_ARROW);
    app->backend.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    // app->backend.lpszMenuName = NULL;	
    // app->backend.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    cassert_always(RegisterClassEx(&app->backend));

    return app;
}

void
ui_child_add(Ui self, UiChild child) {

}

void
ui_mainloop(Ui self) {
    // STARTUPINFO startup_info;
    // /// Specifies the window station, desktop, standard handles, and appearance of the main window for a process at creation time.
    // GetStartupInfo(&startup_info);  
    // int nCmdShow = startup_info.wShowWindow;

    self->is_activated = true;
    // self->on_activate_handler = on_activate_handler;
    // self->on_activate_handler(self);

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
ui_destroy(Ui* self) {
    cassert(self && *self);

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
            // UiBackend* self = (UiBackend*)(((CREATESTRUCT*)lParam)->lpCreateParams);
            // self->on_activate_handler(self);
            break;
        case WM_CLOSE:		
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_MOVE:
            break;
        case WM_SIZE:
            break;
        case WM_COMMAND:
            switch(HIWORD(wParam)) {
                case BN_CLICKED:
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

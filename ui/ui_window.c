#include <ui_window.h>
#include <ui_internal_win/ui_internal_win_window.h>
#include <cassert.h>

UIWindow
ui_window_add(UiBackend self, const char* title, size_t title_len, size_t width, size_t height) {
    cassert(self);
    cassert(title);
    cassert(title_len > 0);
    cassert(title[title_len] == '\0');

    HWND window = CreateWindowA(
        self->backend.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        self->backend.hInstance,
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
ui_window_visiable(UIWindow window, bool visiable) {
    cassert(window);

    // STARTUPINFO startup_info;
    // /// Specifies the window station, desktop, standard handles, and appearance of the main window for a process at creation time.
    // GetStartupInfo(&startup_info);  
    // int nCmdShow = startup_info.wShowWindow;

    // cassert_always(ShowWindow(window, nCmdShow));
    // cassert_always(UpdateWindow(window));
}

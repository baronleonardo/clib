#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <stdbool.h>

#include <ui.h>

typedef void* UIWindow;


UIWindow
ui_window_add(Ui self, const char* title, size_t title_len, size_t width, size_t height);


void
ui_window_visiable(UIWindow window, bool visiable);

#endif // UI_WINDOW_H

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <stdbool.h>

#include <ui.h>

typedef void UiWindow;


UiWindow*
ui_window_add(Ui* self, const char* title, size_t title_len, size_t width, size_t height);


void
ui_window_child_add(UiWindow* window, UiChild* child);


void
ui_window_show(UiWindow* window, bool is_shown);

#endif // UI_WINDOW_H

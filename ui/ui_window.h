#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <stdbool.h>

#include <ui.h>

typedef void UiWindow;


UiWindow*
ui_window_create(Ui* self, const char* title, size_t title_len, size_t width, size_t height);


void
ui_window_child_add(UiWindow* window, UiWidget* child);


void
ui_window_show(UiWindow* window, bool is_shown);


void
ui_window_destroy(UiWindow** window);

#endif // UI_WINDOW_H

#ifndef UI_LISTBOX_H
#define UI_LISTBOX_H

#include <stdbool.h>
#include <stddef.h>

#include <ui.h>

typedef void UiBox;


UiBox*
ui_box_create(UiWidget* parent, bool is_vertical, size_t spacing);


// void
// ui_box_push(UiBox* self, UiWidget* widget);


void
ui_box_remove(UiBox* self, UiWidget* widget);


void
ui_box_destroy(UiBox** self);

#endif // UI_LISTBOX_H

#ifndef UI_H
#define UI_H

#include <stddef.h>
#include <stdbool.h>

typedef struct UiBackend* Ui;
typedef void* UiWidget;


Ui
ui_create(const char* title, size_t title_len);


void
ui_child_add(Ui self, UiWidget widget);


void
ui_mainloop(Ui self, void on_activate_handler(Ui self));


void
ui_destroy(Ui* self);

#endif // UI_H

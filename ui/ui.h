#ifndef UI_H
#define UI_H

#include <stddef.h>
#include <stdbool.h>

typedef struct UiBackend* Ui;
typedef void* UiWidget;
typedef void* UiButton;


Ui
ui_create(const char* title, size_t title_len);


void
ui_child_add(Ui self, UiWidget widget);


UiButton
ui_button_create(
    Ui self,
    const char* label,
    size_t label_len
);


void
ui_button_event_clicked(UiButton button, void on_click_event(UiButton button, void* extra_data), void* extra_data);


void
ui_mainloop(Ui self, char* argv[], int argc, void on_activate_handler(Ui self));


void
ui_destroy(Ui* self);

#endif // UI_H

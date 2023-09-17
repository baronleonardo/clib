#ifndef UI_H
#define UI_H

#include <stddef.h>
#include <stdbool.h>

typedef struct UiBackend {
    bool is_activated;
    const char* title;
    size_t title_len;
    void* backend;
} UiBackend;
typedef struct UiBackend* Ui;
typedef void* UiChild;


Ui
ui_create(const char* class_name, size_t class_name_len);


void
ui_child_add(Ui self, UiChild child);


void
ui_mainloop(Ui self, void construction_handler(Ui self));


void
ui_destroy(Ui* self);

#endif // UI_H

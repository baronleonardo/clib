#ifndef UI_H
#define UI_H

#include <stddef.h>
#include <stdbool.h>

typedef struct Ui {
    bool is_activated;
    void* backend;
} Ui;
typedef void UiWidget;


Ui*
ui_create(const char* class_name, size_t class_name_len);


void
ui_mainloop(Ui* self, void construction_handler(Ui* self));


void
ui_destroy(Ui** self);

#endif // UI_H

#ifndef UI_BUTTON
#define UI_BUTTON

#include <stddef.h>
#include <stdbool.h>

#include <ui.h>

typedef void UiButton;

UiButton*
ui_button_create(Ui* self, const char* label, size_t label_len);


void
ui_button_event_clicked(
    UiButton* button,
    void on_click_event(UiButton* button, void* extra_data),
    void* extra_data
);

#endif // UI_BUTTON

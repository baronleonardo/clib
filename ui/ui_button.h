#ifndef UI_BUTTON
#define UI_BUTTON

#include <stddef.h>
#include <stdbool.h>

#include <ui.h>

typedef void UiButton;


/// @brief create a new push button
/// @param parent this can be anything starting from UiWindow till UiButton for example
/// @param label 
/// @param label_len 
/// @return 
UiButton*
ui_button_create(UiWidget* parent, const char* label, size_t label_len);


void
ui_button_event_clicked(
    UiButton* button,
    void on_click_event(UiButton* button, void* extra_data),
    void* extra_data
);

/// @brief 
/// @param button 
void
ui_button_destroy(UiButton** button);

#endif // UI_BUTTON

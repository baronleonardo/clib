#include <ui.h>

#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#define STR(str) (str), (sizeof((str)) - 1)

void
on_button_clicked_event(UiButton button, void* extra_data) {
    puts("clicked");
}

void
on_activate_handler(Ui self) {
    UiButton* button = ui_button_create(self, STR("button"));
    ui_button_event_clicked(button, on_button_clicked_event, NULL);
    ui_child_add(self, button);
}

int main(int argc, char* argv[]) {
    Ui ui = ui_create(STR("This is a title"));
    ui_mainloop(ui, argv, argc, on_activate_handler);
    ui_destroy(&ui);
}

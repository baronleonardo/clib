#include <log.h>

int main(void) {
    clog_info("%s", "This is an info");
    clog_warn("%s", "This is a warning");
    clog_error("%s", "This is an error");
    clog_fatal("%s", "This is fatal");
}

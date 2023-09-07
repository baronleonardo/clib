#include <log.h>
#include <log_internal.h>

#include <cassert.h>

void
log_impl(LogType type, const char* file, uint32_t line, FILE* out, bool use_color, const char* format, ...) {
    cassert(type < 4);
    cassert(out);

    const char* log_types[] = { "INFO", "WARN", "ERROR", "FATAL" };
    const char* colors[]    = { "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m" };
    const char* color_reset = "\x1b[0m";

    enum { time_buf_size = 9 };
    char time_buf[time_buf_size];
    log_internal_get_cur_time(time_buf, time_buf_size);

    if(use_color) {
        cassert(fprintf(out, "%s %s%-5s%s %s:%d: ", time_buf, colors[type], log_types[type], color_reset, file, line));
    } else {
        cassert(fprintf(out, "%s %-5s %s:%d: ", time_buf, log_types[type], file, line));
    }

    va_list args;
    va_start(args, format);
    cassert(vfprintf(out, format, args));
    cassert(fprintf(out, "\n"));
    cassert(fflush(out) == 0);
    va_end(args);
}

#include <clib/log.h>

#include <time.h>
#include <assert.h>

static void __log_get_cur_time__(char* time_buf, size_t time_buf_size);

void
logger(Log_Type type, const char* file, uint32_t line, FILE* out, bool use_color, const char* format, ...)
{
    assert(type < 4);

    const char* log_types[] = { "INFO", "WARN", "ERROR", "FATAL" };
    const char* colors[]    = { "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m" };
    const char* color_reset = "\x1b[0m";

    const size_t time_buf_size = 9;
    char time_buf[time_buf_size];
    __log_get_cur_time__(time_buf, time_buf_size);

    if(use_color)
    {
        fprintf(out, "%s %s%-5s%s %s:%d: ", time_buf, colors[type], log_types[type], color_reset, file, line);
    }
    else
    {
        fprintf(out, "%s %-5s %s:%d: ", time_buf, log_types[type], file, line);
    }

    va_list args;
    va_start(args, format);
    vfprintf(out, format, args);
    fprintf(out, "\n");
    fflush(out);
    va_end(args);
}

void
__log_get_cur_time__(char* time_buf, size_t time_buf_size)
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime (time_buf, time_buf_size, "%H:%M:%S", timeinfo);
    time_buf[time_buf_size - 1] = '\0';
}
#ifndef LOG_INTERNAL_H
#define LOG_INTERNAL_H

#include <time.h>

#include <cassert.h>

static inline void
log_internal_get_cur_time(char* time_buf, size_t time_buf_size) {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    cassert(timeinfo);

    cassert(strftime (time_buf, time_buf_size, "%H:%M:%S", timeinfo));
    time_buf[time_buf_size - 1] = '\0';
}

#endif // LOG_INTERNAL_H

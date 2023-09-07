#ifndef LOG_INTERNAL_H
#define LOG_INTERNAL_H

#include <time.h>

#include <cassert.h>

static inline void
log_internal_get_cur_time(char* time_buf, size_t time_buf_size) {
    time_t rawtime;
    struct tm timeinfo;

    time(&rawtime);

#if defined(__unix__)
    localtime_r(&rawtime, &timeinfo);
#elif defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
    localtime_s(&timeinfo, &rawtime);
#else
    timeinfo = localtime(&rawtime);
    cassert(timeinfo);
#endif

    cassert(strftime (time_buf, time_buf_size, "%H:%M:%S", &timeinfo));
    time_buf[time_buf_size - 1] = '\0';
}

#endif // LOG_INTERNAL_H

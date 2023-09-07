#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef enum { LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL } LogType;

void
log_impl(LogType type, const char* file, uint32_t line, FILE* out, bool use_color, const char* format, ...);

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// log to stdout/stderr
#define log_info(fmt, ...)  (log_impl(LOG_INFO,  __FILENAME__, __LINE__, stdout, false, (fmt), __VA_ARGS__))
#define log_warn(fmt, ...)  (log_impl(LOG_WARN,  __FILENAME__, __LINE__, stderr, false, (fmt), __VA_ARGS__))
#define log_error(fmt, ...) (log_impl(LOG_ERROR, __FILENAME__, __LINE__, stderr, false, (fmt), __VA_ARGS__))
#define log_fatal(fmt, ...) (log_impl(LOG_FATAL, __FILENAME__, __LINE__, stderr, false, (fmt), __VA_ARGS__))

// colorful logging to stdout/stderr
#define clog_info(fmt, ...)  (log_impl(LOG_INFO,  __FILENAME__, __LINE__, stdout, true, (fmt), __VA_ARGS__))
#define clog_warn(fmt, ...)  (log_impl(LOG_WARN,  __FILENAME__, __LINE__, stderr, true, (fmt), __VA_ARGS__))
#define clog_error(fmt, ...) (log_impl(LOG_ERROR, __FILENAME__, __LINE__, stderr, true, (fmt), __VA_ARGS__))
#define clog_fatal(fmt, ...) (log_impl(LOG_FATAL, __FILENAME__, __LINE__, stderr, true, (fmt), __VA_ARGS__))

// log to file
#define flog_info(out, fmt, ...)  (log_impl(LOG_INFO,  __FILENAME__, __LINE__, (out), false, (fmt), __VA_ARGS__))
#define flog_warn(out, fmt, ...)  (log_impl(LOG_WARN,  __FILENAME__, __LINE__, (out), false, (fmt), __VA_ARGS__))
#define flog_error(out, fmt, ...) (log_impl(LOG_ERROR, __FILENAME__, __LINE__, (out), false, (fmt), __VA_ARGS__))
#define flog_fatal(out, fmt, ...) (log_impl(LOG_FATAL, __FILENAME__, __LINE__, (out), false, (fmt), __VA_ARGS__))

#endif // LOG_H

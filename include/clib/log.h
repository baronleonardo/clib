#ifndef CLIB_LOG_H
#define CLIB_LOG_H

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef enum { CLOG_INFO, CLOG_WARN, CLOG_ERROR, CLOG_FATAL } CLog_Type;

void c_log_impl(CLog_Type type, const char* file, uint32_t line, FILE* out, bool use_color, const char* format, ...);

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// log to stdout/stderr
#define c_log_info(fmt, ...)  c_log_impl(CLOG_INFO,  __FILENAME__, __LINE__, stdout, false, fmt, __VA_ARGS__)
#define c_log_warn(fmt, ...)  c_log_impl(CLOG_WARN,  __FILENAME__, __LINE__, stderr, false, fmt, __VA_ARGS__)
#define c_log_error(fmt, ...) c_log_impl(CLOG_ERROR, __FILENAME__, __LINE__, stderr, false, fmt, __VA_ARGS__)
#define c_log_fatal(fmt, ...) c_log_impl(CLOG_FATAL, __FILENAME__, __LINE__, stderr, false, fmt, __VA_ARGS__)

// colorful logging to stdout/stderr
#define c_clog_info(fmt, ...)  c_log_impl(CLOG_INFO,  __FILENAME__, __LINE__, stdout, true, fmt, __VA_ARGS__)
#define c_clog_warn(fmt, ...)  c_log_impl(CLOG_WARN,  __FILENAME__, __LINE__, stderr, true, fmt, __VA_ARGS__)
#define c_clog_error(fmt, ...) c_log_impl(CLOG_ERROR, __FILENAME__, __LINE__, stderr, true, fmt, __VA_ARGS__)
#define c_clog_fatal(fmt, ...) c_log_impl(CLOG_FATAL, __FILENAME__, __LINE__, stderr, true, fmt, __VA_ARGS__)

// log to file
#define c_flog_info(out, fmt, ...)  c_log_impl(CLOG_INFO,  __FILENAME__, __LINE__, out, false, fmt, __VA_ARGS__)
#define c_flog_warn(out, fmt, ...)  c_log_impl(CLOG_WARN,  __FILENAME__, __LINE__, out, false, fmt, __VA_ARGS__)
#define c_flog_error(out, fmt, ...) c_log_impl(CLOG_ERROR, __FILENAME__, __LINE__, out, false, fmt, __VA_ARGS__)
#define c_flog_fatal(out, fmt, ...) c_log_impl(CLOG_FATAL, __FILENAME__, __LINE__, out, false, fmt, __VA_ARGS__)

#endif // CLIB_LOG_H
#ifndef CLIB_LOG_H
#define CLIB_LOG_H

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum { LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL } Log_Type;

void logger(Log_Type type, const char* file, uint32_t line, FILE* out, bool use_color, const char* format, ...);

// log to stdout/stderr
#define log_info(fmt, ...)  logger(LOG_INFO,  __FILE__, __LINE__, stdout, false, fmt, __VA_ARGS__)
#define log_warn(fmt, ...)  logger(LOG_WARN,  __FILE__, __LINE__, stderr, false, fmt, __VA_ARGS__)
#define log_error(fmt, ...) logger(LOG_ERROR, __FILE__, __LINE__, stderr, false, fmt, __VA_ARGS__)
#define log_fatal(fmt, ...) logger(LOG_FATAL, __FILE__, __LINE__, stderr, false, fmt, __VA_ARGS__)

// colorful logging to stdout/stderr
#define clog_info(fmt, ...)  logger(LOG_INFO,  __FILE__, __LINE__, stdout, true, fmt, __VA_ARGS__)
#define clog_warn(fmt, ...)  logger(LOG_WARN,  __FILE__, __LINE__, stderr, true, fmt, __VA_ARGS__)
#define clog_error(fmt, ...) logger(LOG_ERROR, __FILE__, __LINE__, stderr, true, fmt, __VA_ARGS__)
#define clog_fatal(fmt, ...) logger(LOG_FATAL, __FILE__, __LINE__, stderr, true, fmt, __VA_ARGS__)

// log to file
#define flog_info(out, fmt, ...)  logger(LOG_INFO,  __FILE__, __LINE__, out, false, fmt, __VA_ARGS__)
#define flog_warn(out, fmt, ...)  logger(LOG_WARN,  __FILE__, __LINE__, out, false, fmt, __VA_ARGS__)
#define flog_error(out, fmt, ...) logger(LOG_ERROR, __FILE__, __LINE__, out, false, fmt, __VA_ARGS__)
#define flog_fatal(out, fmt, ...) logger(LOG_FATAL, __FILE__, __LINE__, out, false, fmt, __VA_ARGS__)

#endif // CLIB_LOG_H
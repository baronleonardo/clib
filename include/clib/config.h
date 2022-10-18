#ifndef CLIB_CONFIG_H
#define CLIB_CONFIG_H

// Error
#define ERROR_MSG_LEN           250
#define ERROR_MEM_ALLOC         "Memory re/allocation failed!"
#define ERROR_MSG_ENABLE

// log
#ifdef _WIN32
#define c_log(type)             c_log_##type
#define C_UNITTEST_NO_COLOR
#else
#define c_log(type)             c_clog_##type
#endif // _WIN32

#endif // CLIB_CONFIG_H
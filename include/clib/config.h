#ifndef CLIB_CONFIG_H
#define CLIB_CONFIG_H

// Error
#define ERROR_MSG_LEN       250
#define ERROR_MEM_ALLOC     "Memory re/allocation failed!"
#define ERROR_MSG_ENABLE

// log
#define c_log(type)         c_clog_##type

#endif // CLIB_CONFIG_H
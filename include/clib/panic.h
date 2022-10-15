#ifndef CLIB_PANIC_H
#define CLIB_PANIC_H

#include "log.h"
#include "config.h"

#define panic(msg) (c_log(fatal)(msg, ""), abort())

#endif // CLIB_PANIC_H
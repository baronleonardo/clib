#ifndef PANIC_H
#define PANIC_H

#include <stdio.h>
#include <stdlib.h>

#define panic(msg) (fprintf(stderr, "Panic: %s", (msg)), abort())

#define panic_if(expr, msg) (!(expr) \
    ? (fprintf(stderr, "Panic: %s:%d @%s\n%s\n", __FILE__, __LINE__, __func__, (msg)), abort()) \
    : (void)0)

#define panic_if_memory_failed(expr) panic_if(expr, "Memory allocation/reallocation failed")

#endif // PANIC_H

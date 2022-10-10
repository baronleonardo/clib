#ifndef CLIB_LIST_H
#define CLIB_LIST_H

#include "typedef.h"

typedef struct CList {
    void* data;
    u32 size;
    u32 capacity;
} CList;

#endif // CLIB_LIST_H
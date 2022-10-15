#include <clib/error.h>
#include <clib/config.h>
#include <clib/log.h>
#include <clib/assert.h>

#include <string.h>

void
c_error_set(CError* self, i32 code, const char* msg)
{
    if(self)
    {
        self->code = code;
#ifdef ERROR_MSG_ENABLE
        self->msg = msg;
#else
        self->msg = "";
#endif
    }
}

void
c_error_set_no_error(CError* self)
{
    if(self)
    {
        self->code = C_NO_ERROR;
        self->msg = "";
    }
}
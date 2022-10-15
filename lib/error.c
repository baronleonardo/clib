#include <clib/error.h>
#include <clib/config.h>
#include <clib/log.h>
#include <clib/assert.h>

#include <string.h>

static struct {
    bool is_old;
    i32 code;
    CString str;
    char msg[ERROR_MSG_LEN];    // this must be after CString struct
} __cerror__ = { .str.size = ERROR_MSG_LEN, .str.str_len = 0, .code = 0, .is_old = false };

CError
c_error_get()
{
    if(!__cerror__.is_old)
    {
        CError err = {
            .code = __cerror__.code,
            .msg = __cerror__.msg,
        };

        __cerror__.is_old = true;

        return err;
    }
    else
    {
        return (CError){};
    }
}

void
c_error_set(i32 code, const char* msg)
{
    u32 msg_size = strlen(msg);
    void* copy_status = memcpy(__cerror__.msg, msg, msg_size);
    if(copy_status)
    {
        __cerror__.is_old = false;
        __cerror__.str.str_len = msg_size;
        __cerror__.code = code;
    }
    else
    {
        c_log(fatal)("Failed to set error due to memcpy error", "");
        abort();
    }
}
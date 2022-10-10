#include <clib/error.h>
#include <clib/config.h>

#include <string.h>

static char __cerror_msg__[ERROR_MSG_LEN];
static u32 __cerror_msg_size__ = 0;
static i32 __cerror_code__ = 0;
static bool __cerror_is_old_error__ = false;

CError
c_error_get()
{
    if(!__cerror_is_old_error__)
    {
        CError err = {
            .code = __cerror_code__,
            .msg = c_string_new(__cerror_msg__, __cerror_msg_size__)
        };

        __cerror_is_old_error__ = true;

        return err;
    }
    else
    {
        return (CError){};
    }
}

bool
c_error_set(const char* msg, u32 msg_size, i32 code)
{
    void* copy_status = memcpy(__cerror_msg__, msg, msg_size);
    if(copy_status)
    {
        __cerror_is_old_error__ = false;
        __cerror_msg_size__ = msg_size;
        __cerror_code__ = code;
        return true;
    }
    
    return false;
}
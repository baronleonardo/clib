#include <clib/string.h>
#include <clib/log.h>

#include <stdlib.h>

/// Private
static u32 __c_string_get_len__(const char* buf, u32 size);

CString
c_string_new(char* buf, u32 size)
{
    CString str = {
        .buf = buf,
        .size = size,
        .len = __c_string_get_len__(buf, size),
    };

    if(str.valid)
    {
        log_fatal("%s", "Empty Buf");
        abort();
    }

    return str;
}

i64
c_string_find(CString* self, CString* token)
{
    /// @TODO: Implement
    log_fatal("%s", "Not implemented");
    abort();
}

i64
c_string_find_char(CString* self, CChar* ch)
{
    /// @TODO: Implement
    log_fatal("%s", "Not implemented");
    abort();
}

void
c_string_free(CString* self)
{
    if(self)
    {
        if(self->valid) free(self->buf);
        self->size = 0;
        self->len = 0;
    }
}

///////////////////////////////////////////////////////////////////

u32
__c_string_get_len__(const char* buf, u32 size)
{
    u32 len = 0;
    uchar ch = buf[0];
    for (u32 iii = 1; ch != 0; ++len, ++iii)
    {
        if ((ch & 0x80))
        {
            if (ch < 0xC0)   // Invalid increment
            {
                return 0;
            }

            ch >>= 4;

            if (ch == 12)
            {
                ch++;
            }

            iii += ch - 12;
        }

        ch = buf[iii];
    }

    return len;
}
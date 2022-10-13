#include <clib/string.h>
#include <clib/log.h>
#include <clib/assert.h>
#include <clib/config.h>
#include <clib/error.h>

#include <stdlib.h>

typedef struct CString {
    u32 size; // including header and zero terminated
    u32 str_len;
} CString;

/// Private
static u32 __c_string_get_len__(const char* buf, u32 size);

cstr
c_string_new(u32 size)
{
    CString* cstring_header = malloc(sizeof(CString) + size + 1);
    c_assert(cstring_header, ERROR_MEM_ALLOC);
    cstring_header->size = sizeof(CString) + size + 1;

    cstr cstr_buf = (cstr)(cstring_header + 1);
    cstring_header->str_len = 0;

    cstr_buf[0] = '\0';

    return cstr_buf;
}

cstr
c_string_new_from_buf(const char* buf, u32 size)
{
    CString* cstring_header = malloc(sizeof(CString) + size + 1);
    c_assert(cstring_header, ERROR_MEM_ALLOC);
    cstring_header->size = sizeof(CString) + size + 1;

    cstr cstr_buf = (cstr)(cstring_header + 1);

    void* mem_copy_status = memcpy(cstr_buf, buf, size);
    c_assert(mem_copy_status, ERROR_MEM_ALLOC);
    cstring_header->str_len = size;

    cstr_buf[size] = '\0';

    return cstr_buf;
}

cstr
c_string_copy(cstr self)
{
    CString* old_cstring_header = (CString*)self - 1;
    CString* new_cstring_header = malloc(old_cstring_header->size);
    c_assert(new_cstring_header, ERROR_MEM_ALLOC);

    void* mem_copy_status = memcpy(new_cstring_header, old_cstring_header, old_cstring_header->size);
    c_assert(mem_copy_status, ERROR_MEM_ALLOC);

    return (cstr)(new_cstring_header + 1);
}

u32
c_string_len(cstr self)
{
    CString* cstring_header = (CString*)self - 1;
    return cstring_header->str_len;
}

u32
c_string_capacity(cstr self)
{
    CString* cstring_header = (CString*)self - 1;
    return cstring_header->size - sizeof(CString) - 1;
}

void
c_string_update_len(cstr self)
{
    CString* cstring_header = (CString*)self - 1;
    u32 new_len = strlen(self);

    if(cstring_header->size - sizeof(CString) - 1 > new_len)
    {
        cstring_header->str_len = new_len;
    }
    else
    {
        const char* msg = "Error: `new_len` is less the size of the string";
        c_error_set('s', msg, strlen(msg));
    }
}

bool
c_string_equal(cstr str1, cstr str2)
{
    c_assert_debug(str1, "");
    c_assert_debug(str2, "");

    /// @todo reimplement
    int comparison = strcmp(str1, str2);

    return comparison == 0 ? true : false;
}

void
c_string_trim(cstr self, const char* chars, u32 chars_len)
{
    c_string_rtrim(self, chars, chars_len);
    c_string_ltrim(self, chars, chars_len);
}

void
c_string_trim_whitespaces(cstr self)
{
    u32 cstr_whitespaces_len = strlen(cstr_whitespaces);
    c_string_rtrim(self, cstr_whitespaces, cstr_whitespaces_len);
    c_string_ltrim(self, cstr_whitespaces, cstr_whitespaces_len);
}

void
c_string_ltrim(cstr self, const char* chars, u32 chars_len)
{
    bool found = false;
    i64 last_trim_index = -1;
    u32 str_len = c_string_len(self);

    for(u32 iii = 0; iii < str_len; ++iii)
    {
        found = false;

        // check if current char is from `chars`
        for(u32 ch = 0; ch < chars_len; ++ch)
        {
            if(self[iii] == chars[ch])
            {
                self[iii] = '\0';
                found = true;
                last_trim_index = iii + 1;
                continue;
            }
        }
        if(!found) break;
    }

    if(last_trim_index > 0)
    {
        memmove(self, &self[last_trim_index], str_len - last_trim_index + 1);
        c_string_update_len(self);
    }
}

void
c_string_ltrim_whitespaces(cstr self)
{
    u32 cstr_whitespaces_len = strlen(cstr_whitespaces);
    c_string_ltrim(self, cstr_whitespaces, cstr_whitespaces_len);
}

void
c_string_rtrim(cstr self, const char* chars, u32 chars_len)
{
    bool found = false;
    bool is_trimmed = false;

    for(u32 iii = c_string_len(self) - 1; iii >= 0; --iii)
    {
        found = false;

        // check if current char is from `chars`
        for(u32 ch = 0; ch < chars_len; ++ch)
        {
            if(self[iii] == chars[ch])
            {
                self[iii] = '\0';
                found = true;
                is_trimmed = true;
                continue;
            }
        }
        if(!found) break;
    }

    if(is_trimmed)
    {
        c_string_update_len(self);
    }
}

void
c_string_rtrim_whitespaces(cstr self)
{
    u32 cstr_whitespaces_len = strlen(cstr_whitespaces);
    c_string_rtrim(self, cstr_whitespaces, cstr_whitespaces_len);
}

i64
c_string_find(cstr self, char* token)
{
    c_log(fatal)("not implementd!", "");
    abort();
}

i64
c_string_find_char(cstr self, CChar* ch)
{
    c_log(fatal)("not implementd!", "");
    abort();
}

cstr
c_string_free(cstr self)
{
    if(self)
    {
        CString* cstring_header = (CString*)self - 1;
        free(cstring_header);
        self = null;
    }

    return self;
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
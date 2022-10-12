#include <clib/file.h>
#include <clib/assert.h>
#include <clib/error.h>

#include <string.h>

CFile
c_file_open(cstr path, cstr mode)
{
    // c_assert_debug(c_string_len(path) == strlen(path), "");
    CFile file = {};
    file.stream = fopen(path, mode);
    if(!file.stream)
    {
        const char* msg = "Error: Can't open the input path";
        c_error_set('f', msg, sizeof(msg) - 1);
    }

    return file;
}

uchar
c_file_read(CFile* self)
{
    return fgetc(self->stream);
}

u32
c_file_readline(CFile* self, cstr buf)
{
    char* read_status = fgets(buf, c_string_capacity(buf), self->stream);
    if(read_status)
    {
        c_string_update_len(buf);
        return c_string_len(buf);
    }
    else
    {
        return 0;
    }
}

void
c_file_close(CFile* self)
{
    c_assert_debug(self, "");
    c_assert_debug(self->stream, "");
    c_assert_debug(fclose(self->stream) == 0, "");
}
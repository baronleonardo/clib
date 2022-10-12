#include <clib/file.h>
#include <clib/assert.h>

#include <string.h>

CFile
c_file_open(CString* path, const char* mode)
{
    c_assert_debug(path->len == strlen(path->buf), "");
    CFile file = {};
    file.handler = fopen(path->buf, mode);

    return file;
}

void
c_file_close(CFile* self)
{
    c_assert_debug(self, "");
    c_assert_debug(self->handler, "");
    c_assert_debug(fclose(self->handler) == 0, "");
}
#include <clib/file.h>
#include <clib/assert.h>
#include <clib/error.h>

#include <string.h>
#include <errno.h>

static bool is_cpu_little_endian();

#define swap(elem1, elem2) (elem1 = elem1 ^ elem2, elem2 = elem2 ^ elem1, elem1 = elem1 ^ elem2)

CFile
c_file_open(cstr path, cstr mode)
{
    // c_assert_debug(c_string_len(path) == strlen(path), "");
    CFile file = {};
    file.stream = fopen(path, mode);
    if(!file.stream)
    {
        c_error_set(errno, "Error: Can't open the input path");
    }

    return file;
}

uchar
c_file_readchar(CFile* self)
{
    c_assert_debug(self, "");
    i32 read_status = fgetc(self->stream);

    if(read_status == EOF)
    {
        c_error_set(ferror(self->stream), "Couldn't read from file");
    }

    return read_status;
}

u32
c_file_readline(CFile* self, cstr buf)
{
    c_assert_debug(self, "");
    c_assert_debug(buf, "");

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

bool
c_file_writechar(CFile* self, uchar ch)
{
    c_assert_debug(self, "");

    i32 write_status = fputc(ch, self->stream);

    if(write_status == EOF)
    {
        c_error_set(ferror(self->stream), "Couldn't write to file");
        return false;
    }

    return true;
}

bool
c_file_writeline(CFile* self, cstr line)
{
    c_assert_debug(self, "");
    c_assert_debug(line, "");

    fwrite(line, 1, c_string_len(line), self->stream);
}

bool
c_file_seek(CFile* self, u32 pos)
{

}

void
c_file_close(CFile* self)
{
    c_assert_debug(self, "");
    c_assert_debug(self->stream, "");
    c_assert_debug(fclose(self->stream) == 0, "");
}

////////////////////////////////////////////////////////////

u32
c_file_read_impl(CFile* self, void* buf, u32 element_size, u32 elements_num, bool append_zero)
{
    c_assert_debug(self, "");
    c_assert_debug(buf, "");

    u32 chars_read = fread(buf, element_size, elements_num, self->stream);

    if(is_cpu_little_endian && element_size != 1)
    {
        for(u32 iii = 0; iii < chars_read; iii++)
        {
            char* cur_element = buf + iii * element_size;
            for(u32 start = 0, end = element_size - 1; start < end; ++start, --end)
            {
                swap(cur_element[start], cur_element[end]);
            }
        }
    }

    if(append_zero)
    {
        *(char*)(buf + element_size * chars_read) = '\0';
    }

    return chars_read;
}

bool
c_file_write_impl(CFile* self, void* buf, u32 element_size, u32 elements_num)
{
    c_assert_debug(self, "");
    c_assert_debug(buf, "");

    if(is_cpu_little_endian && element_size != 1)
    {
        for(u32 iii = 0; iii < elements_num; iii++)
        {
            char* cur_element = buf + iii * element_size;
            for(u32 start = 0, end = element_size - 1; start < end; ++start, --end)
            {
                swap(cur_element[start], cur_element[end]);
            }
        }
    }

    u32 written_chars = fwrite(buf, element_size, elements_num, self->stream);

    if(written_chars == EOF)
    {
        c_error_set(ferror(self->stream), "Couldn't write to file");
    }

    return written_chars;
}

bool
is_cpu_little_endian()
{
    i32 num = 1;
    return *(char*)&num == 1;
}
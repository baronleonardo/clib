#include <clib/file.h>
#include <clib/assert.h>
#include <clib/error.h>

#include <string.h>

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
        const char* msg = "Error: Can't open the input path";
        c_error_set('f', msg, sizeof(msg) - 1);
    }

    return file;
}

uchar
c_file_readchar(CFile* self)
{
    c_assert_debug(self, "");
    return fgetc(self->stream);
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

void
c_file_writechar(CFile* self, uchar ch)
{

}

void
c_file_writeline(CFile* self, cstr line)
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

u32
c_file_write_impl(CFile* self, void* buf, u32 element_size, u32 elements_num)
{
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

    return written_chars;
}

bool
is_cpu_little_endian()
{
    i32 num = 1;
    return *(char*)&num == 1;
}
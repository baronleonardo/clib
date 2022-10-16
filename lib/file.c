#include <clib/file.h>
#include <clib/assert.h>
#include <clib/error.h>
#include <clib/log.h>
#include <clib/config.h>
#include <clib/panic.h>

#include <string.h>
#include <errno.h>

static bool is_cpu_little_endian();

#define swap(elem1, elem2) (elem1 = elem1 ^ elem2, elem2 = elem2 ^ elem1, elem1 = elem1 ^ elem2)

CFile
c_file_open(cstr path, cstr mode, CError* err)
{
    // c_assert_debug(c_string_len(path) == strlen(path), "");
    CFile file;
#ifdef _WIN32
    errno_t open_err = fopen_s(&file.stream, path, mode);
    if(open_err != 0)
    {
#else
    file.stream = fopen(path, mode);
    if(!file.stream)
    {
#endif
        c_error_set(err, errno, "Error: Can't open the input path");
    }
    else
    {
        c_error_set_no_error(err);
    }

    return file;
}

uchar
c_file_readchar(CFile* self, CError* err)
{
    c_assert_debug(self, "");
    i32 read_status = fgetc(self->stream);

    if(read_status == EOF)
    {
        c_error_set(err, ferror(self->stream), "Couldn't read from file");
    }
    else
    {
        c_error_set_no_error(err);
    }

    return read_status;
}

u32
c_file_readline(CFile* self, cstr buf, CError* err)
{
    c_assert_debug(self, "");
    c_assert_debug(buf, "");

    char* read_status = fgets(buf, c_string_capacity(buf), self->stream);
    if(read_status)
    {
        c_string_update_len(buf, err);
        return c_string_len(buf);
    }
    else
    {
        return 0;
    }
}

void
c_file_writechar(CFile* self, uchar ch, CError* err)
{
    c_assert_debug(self, "");

    i32 write_status = fputc(ch, self->stream);

    if(write_status == EOF)
    {
        c_error_set(err, ferror(self->stream), "Couldn't write to file");
    }
    else
    {
        c_error_set_no_error(err);
    }
}

void
c_file_writeline(CFile* self, cstr line, CError* err)
{
    c_assert_debug(self, "");
    c_assert_debug(line, "");

    u32 chars_written = fwrite(line, 1, c_string_len(line), self->stream);
    if(chars_written == EOF)
    {
        c_error_set(err, ferror(self->stream), "Couldn't write to file");
    }
    else
    {
        c_error_set_no_error(err);
    }
}

void
c_file_seek(CFile* self, u32 pos, CError* err)
{
    panic("not implemented!");
}

void
c_file_close(CFile* self, CError* err)
{
    c_assert_debug(self, "");
    c_assert_debug(self->stream, "");
    if(fclose(self->stream) != 0)
    {
        c_error_set(err, ferror(self->stream), "Couldn't close the file");
    }
    else
    {
        c_error_set_no_error(err);
    }
}

////////////////////////////////////////////////////////////

u32
c_file_read_impl(CFile* self, void* buf, u32 element_size, u32 elements_num, bool append_zero, CError* err)
{
    c_assert_debug(self, "");
    c_assert_debug(buf, "");

    u32 chars_read = fread(buf, element_size, elements_num, self->stream);

    if(is_cpu_little_endian() && element_size != 1)
    {
        for(u32 iii = 0; iii < chars_read; iii++)
        {
            char* cur_element = (char*)buf + iii * element_size;
            for(u32 start = 0, end = element_size - 1; start < end; ++start, --end)
            {
                swap(cur_element[start], cur_element[end]);
            }
        }
    }

    if(append_zero)
    {
        *(char*)((char*)buf + element_size * chars_read) = '\0';
    }

    return chars_read;
}

void
c_file_write_impl(CFile* self, void* buf, u32 element_size, u32 elements_num, CError* err)
{
    c_assert_debug(self, "");
    c_assert_debug(buf, "");

    if(is_cpu_little_endian() && element_size != 1)
    {
        for(u32 iii = 0; iii < elements_num; iii++)
        {
            char* cur_element = (char*)buf + iii * element_size;
            for(u32 start = 0, end = element_size - 1; start < end; ++start, --end)
            {
                swap(cur_element[start], cur_element[end]);
            }
        }
    }

    u32 written_chars = fwrite(buf, element_size, elements_num, self->stream);

    if(written_chars == EOF)
    {
        c_error_set(err, ferror(self->stream), "Couldn't write to file");
    }
    else
    {
        c_error_set_no_error(err);
    }
}

bool
is_cpu_little_endian()
{
    i32 num = 1;
    return *(char*)&num == 1;
}
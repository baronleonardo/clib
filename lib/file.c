#include <clib/file.h>
#include <clib/assert.h>
#include <clib/error.h>
#include <clib/log.h>
#include <clib/config.h>
#include <clib/panic.h>
#include <clib/list.h>

#include <string.h>
#include <errno.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

static bool is_cpu_little_endian();
#ifdef _WIN32
static wchar_t* cstr_to_cwstr(const cstr str, CError* err);
static u32 str_to_wstr(const char* str, wchar_t* res, u32 res_size, CError* err);
#endif // _WIN32

#define swap(elem1, elem2) (elem1 = elem1 ^ elem2, elem2 = elem2 ^ elem1, elem1 = elem1 ^ elem2)

CFile
c_file_open(cstr path, const char* mode, CError* err)
{
    CFile file;
#ifdef _WIN32
    // convert mode to unicode
    enum { wide_mode_size = 10 };
    wchar_t wide_mode[wide_mode_size];
    u32 wide_mode_len = str_to_wstr(mode, wide_mode, wide_mode_size, err);
    // convert path to unicode
    wchar_t* wide_path = cstr_to_cwstr(path, err);

    errno_t open_err = _wfopen_s(&file.stream, wide_path, wide_mode);
    c_list_free(wide_path);
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

#ifdef _WIN32
wchar_t*
cstr_to_cwstr(const cstr str, CError* err)
{
    i32 wstr_len = MultiByteToWideChar(CP_UTF8, 0, str, -1, null, 0);
    if(wstr_len <= 0)
    {
        if(err)
        {
            c_error_set(err, GetLastError(), "Couldn't convert cstr to cwstr");
            return L"";
        }
        else
        {
            c_error_set_no_error(err);
        }
    }

    // cstr result_cwstr = c_string_new(wstr_len * sizeof(wchar_t));
    c_list(wchar_t) result_cwstr = c_list_new_with_capacity(wchar_t, wstr_len);
    wstr_len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, result_cwstr, wstr_len);
    if(wstr_len <= 0)
    {
        if(err)
        {
            c_error_set(err, GetLastError(), "Couldn't convert cstr to cwstr");
            return L"";
        }
        else
        {
            c_error_set_no_error(err);
        }
    }

    return result_cwstr;
}

u32
str_to_wstr(const char* str, wchar_t* res, u32 res_size, CError* err)
{
    u32 str_len = strlen(str);

    i32 wstr_len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, null, 0);
    if(wstr_len <= 0)
    {
        if(err)
        {
            c_error_set(err, GetLastError(), "Couldn't convert cstr to cwstr");
            return wstr_len;
        }
        else
        {
            c_error_set_no_error(err);
        }
    }

    if(wstr_len >= res_size)
    {
        panic("Can not convert str to wstr as the buffer size is smaller than str len");
    }

    wstr_len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, res, wstr_len);
    if(wstr_len <= 0)
    {
        if(err)
        {
            c_error_set(err, GetLastError(), "Couldn't convert cstr to cwstr");
            return wstr_len;
        }
        else
        {
            c_error_set_no_error(err);
        }
    }

    return wstr_len;
}
#endif // _WIN32
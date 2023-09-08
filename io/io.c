#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <dirent.h>
#endif

#include <io.h>
#include <io_internal.h>
#include <cassert.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

File
io_file_open(const char* path, size_t path_len, const char mode[]) {
    cassert(mode);
    cassert(path);
    cassert(path_len > 0);

#if defined(_WIN32)   // MSVC on windows
    FILE* opened_file = NULL;
    cassert_always_perror(fopen_s(&opened_file, path, mode) == 0, path);
#else
    FILE* opened_file = fopen(path, mode);
    cassert_always_perror(opened_file, path);
#endif

    return opened_file;
}

size_t
io_file_size(File self) {
    cassert(self);

    cassert_always_perror(fseek(self, 0, SEEK_END) == 0, "");
    size_t fsize = ftell(self);
    cassert_always_perror(fseek(self, 0, SEEK_SET) == 0 , "");  /* same as rewind(f); */

    return fsize;
}

size_t
io_file_read(File self, char buf[], size_t buf_size) {
    cassert(self);
    cassert(buf);
    cassert(buf_size > 0);

    size_t fsize = io_file_size(self);
    size_t read_amount = MIN(buf_size, fsize);

    size_t read_size = fread(buf, sizeof(buf[0]), read_amount, self);
    cassert_always_perror((ferror(self) == 0) && (feof(self) == 0), "");

    return read_size;
}

size_t
io_file_write(File self, char buf[], size_t buf_size) {
    cassert(self);
    cassert(buf);
    cassert(buf_size > 0);

    size_t write_size = fwrite(buf, sizeof(buf[0]), buf_size, self);
    cassert_always_perror(ferror(self) == 0, "");

    return write_size;
}

void
io_file_close(File* self) {
    cassert(self && *self);
    cassert(fclose(*self) == 0);

    *self = NULL;
}

void
io_delete(const char* path, size_t path_len) {
    cassert(path);
    cassert(path_len > 0);

#if defined(_WIN32)
    size_t dwAttrib = GetFileAttributes(path);

    if(dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
        cassert_always(RemoveDirectoryA(path));
        return;
    }
#endif

    cassert_always_perror((remove(path) == 0), path);
}

void
io_delete_recursively(const char* path, size_t path_len) {
    cassert(path);
    cassert(path_len > 0);

#if defined(_WIN32)
    WIN32_FIND_DATA cur_file;
    HANDLE find_handler;
    enum { buf_len = MAX_PATH };
    char buf[buf_len];

    if(path[path_len - 1] == '\0') {
        path_len--;
    }

    cassert(memcpy_s(buf, buf_len, path, path_len) == 0);
    buf[path_len] = '/';
    buf[path_len + 1] = '*';
    buf[path_len + 2] = '\0';
    path_len += 2;

    find_handler = FindFirstFile(buf, &cur_file);
    do {
        cassert_always(find_handler != INVALID_HANDLE_VALUE);
        // skip '.' and '..'
        if((strncmp(cur_file.cFileName, ".", 1) != 0) && (strncmp(cur_file.cFileName, "..", 2) != 0)) {
            size_t filename_len = strnlen_s(cur_file.cFileName, MAX_PATH);
            cassert(memcpy_s(buf + path_len - 1, buf_len, cur_file.cFileName, filename_len) == 0);

            if(cur_file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                buf[path_len - 1 + filename_len] = '/';
                buf[path_len + filename_len] = '*';
                buf[path_len + 1 + filename_len] = '\0';

                io_delete_recursively(buf, path_len + filename_len);

                buf[path_len - 1 + filename_len] = '\0';
                io_delete(buf, path_len + filename_len);
            } else {
                buf[path_len - 1 + filename_len] = '\0';
                io_delete(buf, path_len + filename_len);
            }
        }
    } while(FindNextFile(find_handler, &cur_file));
   
    FindClose(find_handler);
#else
#endif
}

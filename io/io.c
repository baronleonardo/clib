#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#if defined(__unix__)
#include <dirent.h>
#elif defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
#include <windows.h>
#endif

#include <io.h>
#include <io_internal.h>
#include <cassert.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

File
io_file_open(const char* path, size_t path_len, const char mode[static 1]) {
    cassert(mode);
    cassert(path);
    cassert(path_len > 0);

    FILE* opened_file = fopen(path, mode);
    cassert_always_perror(opened_file, path);

    return opened_file;
}

size_t
io_file_size(File self) {
    cassert(self);

    cassert_always_msg(fseek(self, 0, SEEK_END) == 0, strerror(ferror(self)));
    size_t fsize = ftell(self);
    cassert_always_msg(fseek(self, 0, SEEK_SET) == 0 , strerror(ferror(self)));  /* same as rewind(f); */

    return fsize;
}

size_t
io_file_read(File self, char buf[static 1], size_t buf_size) {
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
io_file_write(File self, char buf[static 1], size_t buf_size) {
    cassert(self);
    cassert(buf);
    cassert(buf_size > 0);

    size_t write_size = fwrite(buf, sizeof(buf[0]), buf_size, self);
    cassert_always_perror((ferror(self) == 0) && (feof(self) == 0), "");

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
    cassert_always_perror((remove(path) == 0), "");
}

void
io_delete_recursively(const char* path, size_t path_len) {
    // ENOTEMPTY
#if defined(__unix__)
    
#elif defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
    
#else
    
#endif
}

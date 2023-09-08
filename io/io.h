#ifndef IO_H
#define IO_H

#include <stdio.h>

typedef FILE* File;

File
io_file_open(const char* path, size_t path_len, const char mode[]);

size_t
io_file_size(File self);

size_t
io_file_read(File self, char buf[], size_t buf_size);

size_t
io_file_write(File self, char buf[], size_t buf_size);

void
io_file_close(File* self);

void
io_delete(const char* path, size_t path_len);

void
io_delete_recursively(const char* path, size_t path_len);

#endif // IO_H

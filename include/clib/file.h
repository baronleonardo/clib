#ifndef CLIB_FILE_H
#define CLIB_FILE_H

#include <clib/string.h>

#include <stdio.h>

typedef struct CFile {
    union
    {
        bool valid;
        FILE* stream;
    };
} CFile;

/*
 * @param mode:
 * "r" -> Opens an existing text file for reading purpose.
 *
 * "w" -> Opens a text file for writing. If it does not exist, then a new file is created. Here your program will start writing content from the beginning of the file.
 *
 * "a" -> Opens a text file for writing in appending mode. If it does not exist, then a new file is created. Here your program will start appending content in the existing file content.
 *	
 * "r+" -> Opens a text file for both reading and writing.
 * 
 * "w+" -> Opens a text file for both reading and writing. It first truncates the file to zero length if it exists, otherwise creates a file if it does not exist.
 * 
 * "a+" -> Opens a text file for both reading and writing. It creates the file if it does not exist. The reading will start from the beginning but writing can only be appended.
 */

CFile
c_file_open(cstr path, cstr mode);

#define c_file_read(self, buf, buf_size, append_zero) c_file_read_impl(self, buf, sizeof(buf[0]), buf_size, append_zero)

uchar
c_file_readchar(CFile* self);

u32
c_file_readline(CFile* self, cstr buf);

/// @note: if this cpu is little endian, the `buf` will be updated
///        take a copy of it first if you want to keep it
#define c_file_write(self, buf, buf_len) c_file_write_impl(self, buf, sizeof(buf[0]), buf_len)

bool
c_file_writechar(CFile* self, uchar ch);

bool
c_file_writeline(CFile* self, cstr line);

bool
c_file_seek(CFile* self, u32 pos);

void
c_file_close(CFile* self);


/// Private functions (DON'T CALL THEM)
u32
c_file_read_impl(CFile* self, void* buf, u32 element_size, u32 elements_num, bool append_zero);

bool
c_file_write_impl(CFile* self, void* buf, u32 element_size, u32 elements_num);

#endif
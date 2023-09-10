#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdbool.h>

typedef FILE* File;

/// @brief open/create a file
/// @param path 
/// @param path_len 
/// @param mode this is the same as mode in `fopen`
/// @return 
File
io_file_open(const char* path, size_t path_len, const char mode[]);


/// @brief get file size
/// @param self 
/// @return 
size_t
io_file_size(File self);


/// @brief read file content
/// @param self 
/// @param buf 
/// @param buf_size 
/// @return return the bytes read
size_t
io_file_read(File self, char buf[], size_t buf_size);


/// @brief write to file
/// @param self 
/// @param buf 
/// @param buf_size 
/// @return return the bytes written
size_t
io_file_write(File self, char buf[], size_t buf_size);


/// @brief close an alreay opend file
/// @param self 
void
io_file_close(File* self);


/// @brief create a directory
/// @param dir_path 
/// @param path_len 
void
io_dir_create(const char* dir_path, size_t path_len);


/// @brief check if path is a directory
/// @param dir_path 
/// @param path_len 
/// @return 
bool
io_dir(const char* dir_path, size_t path_len);


/// @brief check if the directory is empty
/// @param dir_path 
/// @param path_len 
/// @return 
bool
io_dir_empty(const char* dir_path, size_t path_len);


/// @brief check if file/directory exists
/// @param path 
/// @param path_len 
/// @return 
bool
io_exists(const char* path, size_t path_len);


/// @brief delete a file/directory (empty directory only)
/// @param path 
/// @param path_len 
void
io_delete(const char* path, size_t path_len);


/// @brief delete a non-empty directory
/// @param dir_path 
/// @param path_len 
void
io_delete_recursively(const char* dir_path, size_t path_len);


/// @brief this will run `handler` on every file/directory inside `path`
///        [BEWARE this will fail if path is not a dir]
/// @param dir_path 
/// @param path_len 
/// @param handler 
/// @param extra_data [optional] send/recieve extra data to the handler
void
io_foreach(
    const char* dir_path,
    size_t path_len,
    bool handler(const char* path, size_t path_len, void* extra_data),
    void* extra_data
);

#endif // IO_H

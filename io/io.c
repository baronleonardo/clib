#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
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
    cassert(path[path_len] == '\0');

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
io_dir_create(const char* dir_path, size_t path_len) {
    cassert(dir_path);
    cassert(path_len > 0);
    cassert(dir_path[path_len] == '\0');

    if(!io_exists(dir_path, path_len)) {
#if defined(_WIN32)
        cassert_always(path_len < MAX_PATH);
        cassert_always(CreateDirectoryA(dir_path, NULL));
#else
        cassert_always(path_len < PATH_MAX);
        const mode_t mkdir_mode_mask = 0777;
        cassert_always_perror(mkdir(dir_path, mkdir_mode_mask) == 0, dir_path);
#endif
    }
}

bool
io_dir(const char* dir_path, size_t path_len) {
    cassert(dir_path);
    cassert(path_len > 0);
    cassert(dir_path[path_len] == '\0');

#if defined(_WIN32)
    cassert_always(path_len < MAX_PATH);

    size_t path_attributes = GetFileAttributes(dir_path);
    return (path_attributes != INVALID_FILE_ATTRIBUTES && 
            (path_attributes & FILE_ATTRIBUTE_DIRECTORY));
#else
    cassert_always(path_len < PATH_MAX);

    struct stat sb = {0};
    return ((stat(dir_path, &sb) == 0) && S_ISDIR(sb.st_mode));
#endif
}

bool
io_dir_empty(const char* dir_path, size_t path_len) {
    cassert(dir_path);
    cassert(path_len > 0);
    cassert(dir_path[path_len] == '\0');
    cassert_always(io_dir(dir_path, path_len));

#if defined(_WIN32)
    cassert_always(path_len < MAX_PATH);

    WIN32_FIND_DATA cur_file;
    HANDLE find_handler;
    enum { buf_len = MAX_PATH };
    char buf[buf_len];
    int number_of_existing_paths = 0;

    cassert(memcpy_s(buf, buf_len, dir_path, path_len) == 0);
    buf[path_len] = '/';
    buf[path_len + 1] = '*';
    buf[path_len + 2] = '\0';
    path_len += 2;

    find_handler = FindFirstFile(buf, &cur_file);
    do {
        cassert_always(find_handler != INVALID_HANDLE_VALUE);

        if(number_of_existing_paths > 2) break; // skip '.' and '..'
        number_of_existing_paths++;
    } while(FindNextFile(find_handler, &cur_file));
   
    cassert(FindClose(find_handler));

    return ((number_of_existing_paths > 2) ? true : false);
#else
    cassert_always(path_len < PATH_MAX);

    int number_of_existing_paths = 0;
    struct dirent *dir_attributes = {0};

    DIR* dir = opendir(dir_path);
    cassert_always_perror(dir, dir_path);

    while ((dir_attributes = readdir(dir)) != NULL) {
        if(number_of_existing_paths > 2) break;     // skip '.' and '..'
        number_of_existing_paths++;
    }

    cassert_always(closedir(dir) == 0);

    return ((number_of_existing_paths < 2) ? true : false);
#endif
}

bool
io_exists(const char* path, size_t path_len) {
    cassert(path);
    cassert(path_len > 0);
    cassert(path[path_len] == '\0');

#if defined(_WIN32)
    cassert_always(path_len < MAX_PATH);

    size_t ftyp = GetFileAttributesA(path);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;  //something is wrong with your path!

    return true;
#else
    cassert_always(path_len < PATH_MAX);
    struct stat path_stats = {0};
    return (stat(path, &path_stats) != -1);
#endif
}

void
io_delete(const char* path, size_t path_len) {
    cassert(path);
    cassert(path_len > 0);
    cassert(path[path_len] == '\0');

#if defined(_WIN32)
    cassert_always(path_len < MAX_PATH);
    size_t dwAttrib = GetFileAttributes(path);

    if(dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
        cassert_always(RemoveDirectoryA(path));
        return;
    }
#endif
    cassert_always(path_len < MAX_PATH);
    cassert_always_perror((remove(path) == 0), path);
}

void
io_delete_recursively(const char* dir_path, size_t path_len) {
    cassert(dir_path);
    cassert(path_len > 0);
    cassert(dir_path[path_len] == '\0');
    cassert_always(io_dir(dir_path, path_len));

#if defined(_WIN32)
    cassert_always(path_len < MAX_PATH);

    WIN32_FIND_DATA cur_file;
    HANDLE find_handler;
    enum { buf_len = MAX_PATH };
    char buf[buf_len];

    cassert(memcpy_s(buf, buf_len, dir_path, path_len) == 0);
    buf[path_len] = '/';
    buf[path_len + 1] = '*';
    buf[path_len + 2] = '\0';
    path_len += 2;

    find_handler = FindFirstFile(buf, &cur_file);
    do {
        cassert_always(find_handler != INVALID_HANDLE_VALUE);
        size_t filename_len = strnlen_s(cur_file.cFileName, MAX_PATH);

        // skip '.' and '..'
        if((strncmp(cur_file.cFileName, ".", filename_len) != 0) && (strncmp(cur_file.cFileName, "..", filename_len) != 0)) {
            cassert(memcpy_s(buf + path_len - 1, buf_len, cur_file.cFileName, filename_len) == 0);
            buf[path_len - 1 + filename_len] = '\0';

            if(cur_file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                io_delete_recursively(buf, path_len - 1 + filename_len);
            } else {
                io_delete(buf, path_len - 1 + filename_len);
            }
        }
    } while(FindNextFile(find_handler, &cur_file));
    io_delete(dir_path, path_len - 2);
   
    cassert_always(FindClose(find_handler));
#else
    cassert_always(path_len < PATH_MAX);

    DIR *cur_dir;
    struct dirent *cur_dir_properties;
    enum { buf_len = PATH_MAX };
    char buf[buf_len];

    cassert(memcpy(buf, dir_path, path_len));

    cur_dir = opendir(dir_path);
    if (cur_dir) {
        while ((cur_dir_properties = readdir(cur_dir)) != NULL) {
            size_t filename_len = strnlen(cur_dir_properties->d_name, PATH_MAX);

            // skip '.' and '..'
            if((strncmp(cur_dir_properties->d_name, ".", filename_len) != 0) && (strncmp(cur_dir_properties->d_name, "..", filename_len) != 0)) {
                buf[path_len] = '/';
                cassert(memcpy(buf + path_len + 1, cur_dir_properties->d_name, filename_len));
                buf[path_len + 1 + filename_len] = '\0';

                if(io_dir(buf, path_len + 1 + filename_len)) {
                    io_delete_recursively(buf, path_len + 1 + filename_len);
                } else {
                    io_delete(buf, path_len + 1 + filename_len);
                }
            }
        }
        io_delete(dir_path, path_len);

        cassert_always(closedir(cur_dir) != -1);
    }
#endif
}

void
io_foreach(const char* dir_path, size_t path_len, bool handler(const char* path, size_t path_len)) {
    cassert(dir_path);
    cassert(path_len > 0);
    cassert(handler);
    cassert(dir_path[path_len] == '\0');
    cassert_always(io_dir(dir_path, path_len));

#if defined(_WIN32)
    WIN32_FIND_DATA cur_file;
    HANDLE find_handler;
    enum { buf_len = MAX_PATH };
    char buf[buf_len];

    cassert(memcpy_s(buf, buf_len, dir_path, path_len) == 0);
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
            buf[path_len - 1 + filename_len] = '\0';

            if(!handler(buf, path_len - 1 + filename_len)) {
                break;
            }
        }
    } while(FindNextFile(find_handler, &cur_file));
   
    cassert(FindClose(find_handler));
#else
    DIR *cur_dir;
    struct dirent *cur_dir_properties;
    enum { buf_len = PATH_MAX };
    char buf[buf_len];

    cassert(memcpy(buf, dir_path, path_len));

    cur_dir = opendir(dir_path);
    if (cur_dir) {
        while ((cur_dir_properties = readdir(cur_dir)) != NULL) {
            if((strncmp(cur_dir_properties->d_name, ".", 1) != 0) && (strncmp(cur_dir_properties->d_name, "..", 2) != 0)) {
                size_t filename_len = strnlen(cur_dir_properties->d_name, PATH_MAX);
                buf[path_len] = '/';
                cassert(memcpy(buf + path_len + 1, cur_dir_properties->d_name, filename_len));
                buf[path_len + 1 + filename_len] = '\0';

                if(!handler(buf, path_len + filename_len)) {
                    break;
                }
            }

        }

        cassert_always(closedir(cur_dir) != -1);
    }
#endif
}

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

File*
io_file_open(const char* path, size_t path_len, const char mode[]) {
    cassert(mode);
    cassert(path);
    cassert(path_len > 0);
    cassert(path[path_len] == '\0');

#if defined(_WIN32)   // MSVC on windows
    wchar_t* wide_path = NULL;
    (void)utf8_to_unicode(path, path_len, &wide_path);
    wchar_t* wide_mode = NULL;
    (void)utf8_to_unicode(mode, strlen(mode), &wide_mode);

    File* opened_file = _wfsopen(wide_path, wide_mode, _SH_DENYNO);
    cassert_always_perror(opened_file, path);
    free(wide_path);
    free(wide_mode);
#else
    FILE* opened_file = fopen(path, mode);
    cassert_always_perror(opened_file, path);
#endif

    return opened_file;
}

size_t
io_file_size(File* self) {
    cassert(self);

    cassert_always_perror(fseek(self, 0, SEEK_END) == 0, "");
    size_t fsize = ftell(self);
    cassert_always_perror(fseek(self, 0, SEEK_SET) == 0 , "");  /* same as rewind(f); */

    return fsize;
}

size_t
io_file_read(File* self, char buf[], size_t buf_size) {
    cassert(self);
    cassert(buf);
    cassert(buf_size > 0);

    size_t fsize = io_file_size(self);
    size_t read_amount = MIN(buf_size, fsize);

    size_t read_size = fread(buf, sizeof(buf[0]), read_amount, self);
    cassert_always_perror(ferror(self) == 0, "");

    return read_size;
}

size_t
io_file_write(File* self, char buf[], size_t buf_size) {
    cassert(self);
    cassert(buf);
    cassert(buf_size > 0);

    size_t write_size = fwrite(buf, sizeof(buf[0]), buf_size, self);
    cassert_always_perror(ferror(self) == 0, "");

    return write_size;
}

void
io_file_close(File** self) {
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
#else
    cassert_always(path_len < PATH_MAX);
#endif

    bool is_empty = true;
    io_foreach(dir_path, path_len, io_internal_dir_empty_handler, &is_empty);

    return is_empty;
}

bool
io_exists(const char* path, size_t path_len) {
    cassert(path);
    cassert(path_len > 0);
    cassert(path[path_len] == '\0');

#if defined(_WIN32)
    cassert_always(path_len < MAX_PATH);

    size_t path_attributes = GetFileAttributesA(path);
    if (path_attributes == INVALID_FILE_ATTRIBUTES)
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
#else
    cassert_always(path_len < PATH_MAX);
#endif
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
#else
    cassert_always(path_len < PATH_MAX);
#endif

    io_foreach(dir_path, path_len, io_internal_delete_recursively_handler, NULL);
}

void
io_foreach(
    const char* dir_path,
    size_t path_len,
    bool handler(const char* path, size_t path_len, void* extra_data),
    void* extra_data
) {
    cassert(dir_path);
    cassert(path_len > 0);
    cassert(handler);
    cassert(dir_path[path_len] == '\0');
    cassert_always(io_dir(dir_path, path_len));

#if defined(_WIN32)
    WIN32_FIND_DATAW cur_file;
    HANDLE find_handler;
    enum { buf_len = MAX_PATH * sizeof(wchar_t) };
    wchar_t buf[buf_len];

    wchar_t* wide_path = NULL;
    size_t wide_path_len = utf8_to_unicode(dir_path, path_len, &wide_path);

    cassert(memcpy_s(buf, buf_len, wide_path, wide_path_len * sizeof(wchar_t)) == 0);
    buf[wide_path_len] = L'/';
    buf[wide_path_len + 1] = L'*';
    buf[wide_path_len + 2] = L'\0';
    wide_path_len += 2;

    find_handler = FindFirstFileW(buf, &cur_file);
    do {
        cassert_always(find_handler != INVALID_HANDLE_VALUE);
        // skip '.' and '..'
        if((wcscmp(cur_file.cFileName, L".") != 0) && (wcscmp(cur_file.cFileName, L"..") != 0)) {
            size_t filename_len = wcsnlen(cur_file.cFileName, MAX_PATH);
            cassert(memcpy_s(buf + wide_path_len - 1, buf_len, cur_file.cFileName, filename_len * sizeof(wchar_t)) == 0);
            buf[wide_path_len - 1 + filename_len] = L'\0';

            char* u8path = NULL;
            size_t u8path_len = unicode_to_utf8(buf, wide_path_len - 1 + filename_len, &u8path);

            bool handler_status = handler(u8path, u8path_len, extra_data);
            free(u8path);

            if(!handler_status) {
                break;
            }
        }
    } while(FindNextFileW(find_handler, &cur_file));
   
    cassert(FindClose(find_handler));

    free(wide_path);
#else
    DIR *cur_dir;
    struct dirent *cur_dir_properties;
    enum { buf_len = PATH_MAX };
    char buf[buf_len];

    cassert(memcpy(buf, dir_path, path_len));

    cur_dir = opendir(dir_path);
    if (cur_dir) {
        while ((cur_dir_properties = readdir(cur_dir)) != NULL) {
            if((strcmp(cur_dir_properties->d_name, ".") != 0) && (strcmp(cur_dir_properties->d_name, "..") != 0)) {
                size_t filename_len = strlen(cur_dir_properties->d_name);
                buf[path_len] = '/';
                cassert(memcpy(buf + path_len + 1, cur_dir_properties->d_name, filename_len));
                buf[path_len + 1 + filename_len] = '\0';

                if(!handler(buf, path_len + 1 + filename_len, extra_data)) {
                    break;
                }
            }

        }

        cassert_always(closedir(cur_dir) != -1);
    }
#endif
}

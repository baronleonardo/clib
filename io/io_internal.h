#ifndef IO_INTERNAL_H
#define IO_INTERNAL_H

#if defined(_WIN32)
#include <wchar.h>
#include <Windows.h>
#include <stdbool.h>
#endif

#include <io.h>
#include <cassert.h>

#if defined(_WIN32)
static inline size_t
io_internal_path_u8_to_wide(const char path[], size_t path_len, wchar_t** out_wide_path) {
    cassert(path);
    cassert(out_wide_path);
    cassert(path_len > 0);

    size_t wide_path_len = MultiByteToWideChar(CP_UTF8, 0, path, path_len, NULL, 0);
    cassert_always(wide_path_len);

    wchar_t* wide_path = (wchar_t*)malloc((wide_path_len + 1) * sizeof(wchar_t));
    cassert(wide_path);

    wide_path_len = MultiByteToWideChar(CP_UTF8, 0, path, path_len, wide_path, wide_path_len * sizeof(wchar_t));
    wide_path[wide_path_len] = '\0';
    
    *out_wide_path = wide_path;
    return wide_path_len;
}

static inline size_t
io_internal_path_wide_to_u8(const wchar_t wide_path[], size_t wide_path_len, char** out_path) {
    cassert(wide_path);
    cassert(out_path);
    cassert(wide_path_len > 0);

    size_t path_len = WideCharToMultiByte(CP_UTF8, 0, wide_path, wide_path_len, NULL, 0, NULL, NULL);
    cassert_always(path_len);

    char* path = (char*)malloc((path_len + 1) * sizeof(char));
    cassert(wide_path);

    path_len = WideCharToMultiByte(CP_UTF8, 0, wide_path, wide_path_len, path, path_len, NULL, NULL);
    path[path_len] = '\0';
    
    *out_path = path;
    return path_len;
}
#endif // _WIN32

bool
io_internal_delete_recursively_handler(const char* path, size_t path_len, void* extra_data) {
    (void)extra_data;

    if(io_dir(path, path_len)) {
        io_delete_recursively(path, path_len);
        io_delete(path, path_len);
    } else {
        io_delete(path, path_len);
    }

    return true;
}

bool
io_internal_dir_empty_handler(const char* path, size_t path_len, void* extra_data) {
    (void)path;
    (void)path_len;

    *(bool*)extra_data = false; // This dir_path is not empty
    return false;
}

#endif // IO_INTERNAL_H

#pragma once

using SdFileCallBack = void (*)(const char *name, bool is_dir, void *user);
using SdRecursiveFileCallBack = void (*)(const char *path, const char *name, bool is_dir, int depth, void *user);


class SD {
public:
    SD();
    ~SD();
    void scan_dir(const char *path, SdFileCallBack cb, void *user);
    void scan_dir_recursive(const char *path, SdRecursiveFileCallBack cb, void *user);
private:
    void scan_dir_recursive_impl(const char *path, int depth, SdRecursiveFileCallBack cb, void *user);
};

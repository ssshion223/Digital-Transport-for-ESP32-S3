#include "sd.h"
#include "esp_err.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <string>
#ifdef __cplusplus
extern "C" {
#endif
#include "__SPI_SD.h"
#ifdef __cplusplus
}
#endif

SD::SD() {
    esp_err_t err = spi_sd_init();
    if (err != ESP_OK) {
        printf("mount failed\r\n");
    } else {
        printf("mount successful\r\n");
    }


}

SD::~SD() {}

void SD::scan_dir(const char *path, SdFileCallBack cb, void *user) {
    DIR *dir = opendir(path);
    if (!dir) {
        printf("open dir failed: %s\n", path);
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, "System Volume Information") == 0) {
            continue;
        }
        std::string full_path = std::string(path) + "/" + entry->d_name;
        struct stat st;
        if (stat(full_path.c_str(), &st) != 0) {
            continue;
        }
        bool is_dir = S_ISDIR(st.st_mode);
        // if (is_dir) {
        //     printf("[DIR] %s\n", entry->d_name);
        // } else {
        //     printf("[FILE] %s %ld bytes\n", entry->d_name, (long)st.st_size);
        // }

        if (cb) {
            cb(entry->d_name, is_dir, user);
        }
    }
    closedir(dir);
}

void SD::scan_dir_recursive(const char *path, SdRecursiveFileCallBack cb, void *user) {
    scan_dir_recursive_impl(path, 0, cb, user);
}

void SD::scan_dir_recursive_impl(const char *path, int depth, SdRecursiveFileCallBack cb, void *user) {
    DIR *dir = opendir(path);
    if (!dir) {
        printf("open dir failed: %s\n", path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strcmp(entry->d_name, "System Volume Information") == 0) {
            continue;
        }

        std::string full_path = std::string(path) + "/" + entry->d_name;

        struct stat st;
        if (stat(full_path.c_str(), &st) != 0) {
            continue;
        }

        bool is_dir = S_ISDIR(st.st_mode);
        if (cb) {
            cb(full_path.c_str(), entry->d_name, is_dir, depth, user);
        }

        if (is_dir) {
            scan_dir_recursive_impl(full_path.c_str(), depth + 1, cb, user);
        }
    }

    closedir(dir);
}

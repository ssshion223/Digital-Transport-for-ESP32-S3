#pragma once
#include "main_page.h"
#include "music_page.h"
#include <string>
#include <vector>
#include "scan_item.h"
#include "folder_page.h"
#include "digital_decoder.h"
#include "play_page.h"
class UI {
public:
    UI();
    ~UI();
    void init();
private:
    void show_main_page();
    void show_music_page();
    void show_folder_page();
    void show_scan_modal();
    void show_play_page();
    static void scan_file_cb(const char *path, const char *name, bool is_dir, int depth, void *user_data);
    static void scan_timer_cb(lv_timer_t *timer);
    static void close_scan_modal_timer_cb(lv_timer_t *timer);
    lv_obj_t *scan_modal_ = nullptr;
    lv_obj_t *scan_modal_label_ = nullptr;
    static void on_play_page_action(PlayPageAction action, void *user_data);
    static void on_main_page_action(MainPageAction action, void *user_data);
    static void on_music_page_action(MusicPageAction action, void *user_data);
    static void on_folder_page_action(FolderPageAction action, size_t index, void *user_data);
    MainPage main_page_;
    MusicPage music_page_;
    FolderPage folder_page_;
    PlayPage play_page_;
    DigitalDecoder decoder_;
private:
    std::vector<ScanItem> scan_items_;
    int scan_file_count_ = 0;
    std::string current_song_name_;
};

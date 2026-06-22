#include <stdio.h>
#include "ui.h"
#include "lvgl.h"
#include "lv_port.h"
#include "sd.h"
#include <cstring>
#include <string>
#include <vector>
#include "digital_decoder.h"
LV_FONT_DECLARE(song_cjk_16);
// struct ScanItem {
//     std::string path;
//     std::string name;
//     int depth;
//     bool is_dir;
// };
// struct ScanStats {
//     int dirs = 0;
//     int files = 0;
// };

void UI::scan_file_cb(const char *path, const char *name, bool is_dir, int depth, void *user_data) {
    auto *ui = static_cast<UI *>(user_data);
    ui->scan_items_.push_back({path, name, depth, is_dir});
    if (!is_dir) {
        ui->scan_file_count_++;
    }
}

UI::UI() {}
UI::~UI() {}

void UI::init() {
    show_main_page();
    decoder_.start();
}
void UI::show_main_page() {
    lv_obj_t *scr = main_page_.create(on_main_page_action, this);
    lv_screen_load(scr);
}
void UI::show_music_page() {
    lv_obj_t *scr = music_page_.create(on_music_page_action, this);
    lv_screen_load(scr);
}
void UI::show_folder_page() {
    lv_obj_t *scr = folder_page_.create(scan_items_, on_folder_page_action, this);
    lv_screen_load(scr);
}
void UI::show_play_page() {
    lv_obj_t *scr = play_page_.create(current_song_name_, decoder_.is_paused(), on_play_page_action, this);
    lv_screen_load(scr);
}

void UI::on_main_page_action(MainPageAction action, void *user_data) {
    UI *ui = static_cast<UI *>(user_data);
    switch (action)
    {
    case MainPageAction::MUSIC:
        ui->show_music_page();
        break;
    case MainPageAction::SCAN:
        ui->show_scan_modal();
        break;
    default:
        break;
    }
    return;
}
void UI::on_music_page_action(MusicPageAction action, void *user_data) {
    UI *ui = static_cast<UI *>(user_data);
    switch (action)
    {
    case MusicPageAction::BACK:
        ui->show_main_page();
        break;
    case MusicPageAction::FOLDER:
        ui->show_folder_page();
        break;
    case MusicPageAction::PLAYING:
        ui->show_play_page();
        break;
    default:
        break;
    }
    return;
}
static bool end_with_audio_ext(const std::string &path, const char *ext) {
    size_t ext_len = std::strlen(ext);
    if (path.size() < ext_len) {
        return false;
    }
    std::string suffix = path.substr(path.size() - ext_len);
    for (char &c : suffix) {
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
    }
    return suffix == ext;
}

static bool is_supported_audio_file(const std::string &path) {
    return end_with_audio_ext(path, ".wav") || end_with_audio_ext(path, ".flac");
}
void UI::on_folder_page_action(FolderPageAction action, size_t index, void *user_data) {
    auto *ui = static_cast<UI *>(user_data);
    switch (action)
    {
    case FolderPageAction::BACK:
        ui->show_music_page();
        break;
    case FolderPageAction::OPEN_FOLDER:
        // printf("open folder");
        break;
    case FolderPageAction::OPEN_MUSIC_FILE:
        // printf("open file");
        if (!is_supported_audio_file(ui->scan_items_[index].path)) {
            return;
        }
        ui->current_song_name_ = ui->scan_items_[index].name;
        ui->decoder_.play_file_async(ui->scan_items_[index].path.c_str());
        ui->show_play_page();
        break;
    default:
        break;
    }
}

void UI::show_scan_modal() {
    if (scan_modal_) {
        return;
    }
    scan_modal_ = lv_obj_create(lv_layer_top());
    lv_obj_set_size(scan_modal_, 220, 120);
    lv_obj_center(scan_modal_);
    lv_obj_set_style_bg_color(scan_modal_, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(scan_modal_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(scan_modal_, lv_color_black(), 0);
    lv_obj_set_style_border_width(scan_modal_, 3, 0);
    lv_obj_set_style_radius(scan_modal_, 0, 0);
    lv_obj_set_style_shadow_width(scan_modal_, 0, 0);
    scan_modal_label_ = lv_label_create(scan_modal_);
    lv_obj_set_style_text_font(scan_modal_label_, &song_cjk_16, 0);
    lv_label_set_text(scan_modal_label_, "扫描中...");
    lv_obj_center(scan_modal_label_);

    lv_timer_t *timer = lv_timer_create(scan_timer_cb, 50, this);
    lv_timer_set_repeat_count(timer, 1);
}

void UI::on_play_page_action(PlayPageAction action, void *user_data) {
    UI *ui = static_cast<UI *>(user_data);
    switch (action)
    {
    case PlayPageAction::BACK:
        ui->show_music_page();
        break;
    case PlayPageAction::PAUSE:
        ui->play_page_.set_paused(ui->decoder_.toggle_pause());
        break;
    default:
        break;
    }
}

void UI::scan_timer_cb(lv_timer_t *timer) {
    auto *ui = static_cast<UI *>(lv_timer_get_user_data(timer));
    if (!ui) return;
    ui->scan_items_.clear();
    ui->scan_file_count_ = 0;
    SD sd;
    sd.scan_dir_recursive("/sdcard", scan_file_cb, ui);

    if (ui->scan_modal_label_) {
        lv_label_set_text_fmt(ui->scan_modal_label_, "完成!\n文件:%d", ui->scan_file_count_);
        lv_obj_center(ui->scan_modal_label_);
    }
    lv_timer_t *close_timer = lv_timer_create(close_scan_modal_timer_cb, 800, ui);
    lv_timer_set_repeat_count(close_timer, 1);
}
void UI::close_scan_modal_timer_cb(lv_timer_t *timer) {
    auto *ui = static_cast<UI *>(lv_timer_get_user_data(timer));
    if (!ui) return;
    if (ui->scan_modal_) {
        lv_obj_delete(ui->scan_modal_);
        ui->scan_modal_ = nullptr;
        ui->scan_modal_label_ = nullptr;
    }
}

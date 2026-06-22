#include "main_page.h"
#include "lvgl.h"
LV_FONT_DECLARE(song_cjk_16);
LV_FONT_DECLARE(ui_cjk_40);
MainPage::MainPage() {}
MainPage::~MainPage() {}

lv_obj_t *MainPage::create(MainPageActionCallBack cb, void *user_data) {
    lv_obj_t *scr = lv_obj_create(NULL);
    cb_ = cb;
    user_data_ = user_data;

    lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *btn_music = lv_button_create(scr);
    lv_obj_add_event_cb(btn_music, on_btn_clikcked, LV_EVENT_CLICKED, &music_ctx_);
    lv_obj_set_size(btn_music, 130, 210);
    lv_obj_set_pos(btn_music, 20, 20);
    lv_obj_set_style_radius(btn_music, 0, 0);
    lv_obj_set_style_bg_color(btn_music, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn_music, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_music, 3, 0);
    lv_obj_set_style_border_color(btn_music, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn_music, 0, 0);
    lv_obj_t *music_label = lv_label_create(btn_music);
    lv_obj_set_style_text_color(music_label, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_font(music_label, &ui_cjk_40, LV_PART_MAIN);
    lv_label_set_text(music_label, "音\n乐");
    lv_obj_center(music_label);

    lv_obj_t *btn_scan = lv_button_create(scr);
    lv_obj_add_event_cb(btn_scan, on_btn_clikcked, LV_EVENT_CLICKED, &scan_ctx_);
    lv_obj_set_size(btn_scan, 130, 210);
    lv_obj_set_pos(btn_scan, 20+130+20, 20);
    lv_obj_set_style_radius(btn_scan, 0, 0);
    lv_obj_set_style_bg_color(btn_scan, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn_scan, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_scan, 3, 0);
    lv_obj_set_style_border_color(btn_scan, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn_scan, 0, 0);
    lv_obj_t *scan_label = lv_label_create(btn_scan);
    lv_obj_set_style_text_color(scan_label, lv_color_black(), 0);
    lv_obj_set_style_text_font(scan_label, &ui_cjk_40, 0);
    lv_label_set_text(scan_label, "扫\n描");
    lv_obj_center(scan_label);

    lv_obj_t *btn2 = lv_button_create(scr);
    lv_obj_add_event_cb(btn2, on_btn_clikcked, LV_EVENT_CLICKED, &btn2_ctx_);
    lv_obj_set_size(btn2, 130, 210);
    lv_obj_set_pos(btn2, 20, 20+210+20);
    lv_obj_set_style_radius(btn2, 0, 0);
    lv_obj_set_style_bg_color(btn2, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn2, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn2, 3, 0);
    lv_obj_set_style_border_color(btn2, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn2, 0, 0);

    lv_obj_t *btn_setting = lv_button_create(scr);
    lv_obj_add_event_cb(btn_setting, on_btn_clikcked, LV_EVENT_CLICKED, &setting_ctx_);
    lv_obj_set_size(btn_setting, 130, 210);
    lv_obj_set_pos(btn_setting, 20+130+20, 20+210+20);
    lv_obj_set_style_radius(btn_setting, 0, 0);
    lv_obj_set_style_bg_color(btn_setting, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn_setting, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_setting, 3, 0);
    lv_obj_set_style_border_color(btn_setting, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn_setting, 0, 0);
    lv_obj_t *setting_label = lv_label_create(btn_setting);
    lv_obj_set_style_text_color(setting_label, lv_color_black(), 0);
    lv_obj_set_style_text_font(setting_label, &ui_cjk_40, LV_PART_MAIN);
    lv_label_set_text(setting_label, "设\n置");
    lv_obj_center(setting_label);
    return scr;
}
void MainPage::on_btn_clikcked(lv_event_t *e) {
    auto *ctx = static_cast<ButtonCtx *>(lv_event_get_user_data(e));
    if (ctx && ctx->page) {
        ctx->page->emit(ctx->action);
    }
}
void MainPage::emit(MainPageAction action) {
    if (cb_) {
        cb_(action, user_data_);
    }
}

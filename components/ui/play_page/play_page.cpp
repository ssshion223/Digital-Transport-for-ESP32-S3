#include "play_page.h"
#include "lvgl.h"
LV_FONT_DECLARE(ui_cjk_40);
LV_FONT_DECLARE(song_cjk_16);
PlayPage::PlayPage() {}
PlayPage::~PlayPage() {}

lv_obj_t *PlayPage::create(const std::string &song_name, bool paused, PlayPageActionCallBack cb, void *user_data) {
    cb_ = cb;
    user_data_ = user_data;
    paused_ = paused;
    pause_label_ = nullptr;
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *back_btn = lv_button_create(scr);
    lv_obj_add_event_cb(back_btn, on_btn_clicked, LV_EVENT_CLICKED, &back_ctx_);
    lv_obj_set_size(back_btn, 50, 40);
    lv_obj_set_pos(back_btn, 0, 0);
    lv_obj_set_style_radius(back_btn, 0, 0);
    lv_obj_set_style_shadow_width(back_btn, 0, 0);
    lv_obj_set_style_bg_color(back_btn, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(back_btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(back_btn, 0, 0);
    lv_obj_set_style_border_color(back_btn, lv_color_black(), 0);
    lv_obj_t *arrow = lv_label_create(back_btn);
    lv_obj_set_style_text_color(arrow, lv_color_black(), 0);
    lv_label_set_text(arrow, LV_SYMBOL_LEFT);
    lv_obj_center(arrow);
    lv_obj_t *page_label = lv_label_create(scr);
    lv_label_set_text(page_label, "播放页");
    lv_obj_set_style_text_font(page_label, &ui_cjk_40, 0);
    lv_obj_set_pos(page_label, 50, 0);

    lv_obj_t *divider = lv_obj_create(scr);
    lv_obj_remove_style_all(divider);
    lv_obj_set_size(divider, 320, 3);
    lv_obj_set_pos(divider, 0, 41);
    lv_obj_set_style_bg_color(divider, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(divider, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *song_label = lv_label_create(scr);
    lv_obj_set_style_text_font(song_label, &song_cjk_16, 0);
    lv_obj_set_style_text_color(song_label, lv_color_black(), 0);
    lv_obj_set_width(song_label, 280);
    lv_label_set_long_mode(song_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(song_label, song_name.empty() ? "未选择歌曲" : song_name.c_str());
    lv_obj_set_pos(song_label, 20, 90);

    lv_obj_t *pause_btn = lv_button_create(scr);
    lv_obj_add_event_cb(pause_btn, on_btn_clicked, LV_EVENT_CLICKED, &pause_ctx_);
    lv_obj_set_size(pause_btn, 120, 70);
    lv_obj_set_pos(pause_btn, 100, 220);
    lv_obj_set_style_radius(pause_btn, 0, 0);
    lv_obj_set_style_shadow_width(pause_btn, 0, 0);
    lv_obj_set_style_bg_color(pause_btn, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(pause_btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(pause_btn, 3, 0);
    lv_obj_set_style_border_color(pause_btn, lv_color_black(), 0);

    pause_label_ = lv_label_create(pause_btn);
    lv_obj_set_style_text_font(pause_label_, &song_cjk_16, 0);
    lv_obj_set_style_text_color(pause_label_, lv_color_black(), 0);
    set_paused(paused_);
    return scr;
}

void PlayPage::on_btn_clicked(lv_event_t *e) {
    auto *ctx = static_cast<ButtonCtx *>(lv_event_get_user_data(e));
    if (ctx && ctx->page) {
        ctx->page->emit(ctx->action);
    }
}

void PlayPage::emit(PlayPageAction action) {
    if (cb_) {
        cb_(action, user_data_);
    }
}

void PlayPage::set_paused(bool paused) {
    paused_ = paused;
    if(pause_label_) {
        lv_label_set_text(pause_label_, paused_ ? "继续" : "暂停");
        lv_obj_center(pause_label_);
    }
}

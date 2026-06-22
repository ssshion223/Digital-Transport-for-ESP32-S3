#include "music_page.h"
#include "lvgl.h"
LV_FONT_DECLARE(ui_cjk_40);

MusicPage::MusicPage() {}
MusicPage::~MusicPage() {}

lv_obj_t *MusicPage::create(MusicPageActionCallBack cb, void *user_data) {
    cb_ = cb;
    user_data_ = user_data;
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *btn_back = lv_button_create(scr);
    lv_obj_add_event_cb(btn_back, on_btn_clicked, LV_EVENT_CLICKED, &back_ctx_);
    lv_obj_set_size(btn_back, 50, 40);
    lv_obj_set_pos(btn_back, 0, 0);
    lv_obj_set_style_radius(btn_back, 0, 0);
    lv_obj_set_style_shadow_width(btn_back, 0, 0);
    lv_obj_set_style_bg_color(btn_back, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn_back, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_back, 0, 0);
    lv_obj_set_style_border_color(btn_back, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn_back, 0, 0);
    lv_obj_t *arrow = lv_label_create(btn_back);
    lv_obj_set_style_text_color(arrow, lv_color_black(), 0);
    // lv_obj_set_style_text_font(arrow, &ui_cjk_40, 0);
    lv_label_set_text(arrow, LV_SYMBOL_LEFT);
    lv_obj_center(arrow);
    lv_obj_t *page_label = lv_label_create(scr);
    lv_label_set_text(page_label, "音乐");
    lv_obj_set_style_text_font(page_label, &ui_cjk_40, 0);
    lv_obj_set_pos(page_label, 50, 0);

    lv_obj_t *divider = lv_obj_create(scr);
    lv_obj_remove_style_all(divider);
    lv_obj_set_size(divider, 320, 3);
    lv_obj_set_pos(divider, 0, 41);
    lv_obj_set_style_bg_color(divider, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(divider, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *btn_playing = lv_button_create(scr);
    lv_obj_add_event_cb(btn_playing, on_btn_clicked, LV_EVENT_CLICKED, &playing_ctx_);
    lv_obj_set_size(btn_playing, 130, 130);
    lv_obj_set_pos(btn_playing, 20, 70);
    lv_obj_set_style_radius(btn_playing, 0, 0);
    lv_obj_set_style_shadow_width(btn_playing, 0, 0);
    lv_obj_set_style_bg_color(btn_playing, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn_playing, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_playing, 3, 0);
    lv_obj_set_style_border_color(btn_playing, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn_playing, 0, 0);
    lv_obj_t *playing_label = lv_label_create(btn_playing);
    lv_obj_set_style_text_font(playing_label, &ui_cjk_40, 0);
    lv_obj_set_style_text_color(playing_label, lv_color_black(), 0);
    lv_label_set_text(playing_label, "播放页");
    lv_obj_center(playing_label);

    lv_obj_t *btn_album = lv_button_create(scr);
    // lv_obj_add_event_cb(btn_playing, on_btn_clicked, LV_EVENT_CLICKED, );
    lv_obj_set_size(btn_album, 130, 130);
    lv_obj_set_pos(btn_album, 20+130+20, 70);
    lv_obj_set_style_radius(btn_album, 0, 0);
    lv_obj_set_style_shadow_width(btn_album, 0, 0);
    lv_obj_set_style_bg_color(btn_album, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn_album, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_album, 3, 0);
    lv_obj_set_style_border_color(btn_album, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn_album, 0, 0);
    lv_obj_t *album_label = lv_label_create(btn_album);
    lv_obj_set_style_text_font(album_label, &ui_cjk_40, 0);
    lv_obj_set_style_text_color(album_label, lv_color_black(), 0);
    lv_label_set_text(album_label, "专辑");
    lv_obj_center(album_label);    

    lv_obj_t *btn_folder = lv_button_create(scr);
    lv_obj_add_event_cb(btn_folder, on_btn_clicked, LV_EVENT_CLICKED, &folder_ctx_);
    lv_obj_set_size(btn_folder, 130, 130);
    lv_obj_set_pos(btn_folder, 20, 70+20+130);
    lv_obj_set_style_radius(btn_folder, 0, 0);
    lv_obj_set_style_shadow_width(btn_folder, 0, 0);
    lv_obj_set_style_bg_color(btn_folder, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn_folder, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_folder, 3, 0);
    lv_obj_set_style_border_color(btn_folder, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn_folder, 0, 0);
    lv_obj_t *folder_label = lv_label_create(btn_folder);
    lv_obj_set_style_text_font(folder_label, &ui_cjk_40, 0);
    lv_obj_set_style_text_color(folder_label, lv_color_black(), 0);
    lv_label_set_text(folder_label, "文件夹");
    lv_obj_center(folder_label);

    lv_obj_t *btn_list = lv_button_create(scr);
    // lv_obj_add_event_cb(btn_playing, on_btn_clicked, LV_EVENT_CLICKED, );
    lv_obj_set_size(btn_list, 130, 130);
    lv_obj_set_pos(btn_list, 20+130+20, 70+20+130);
    lv_obj_set_style_radius(btn_list, 0, 0);
    lv_obj_set_style_shadow_width(btn_list, 0, 0);
    lv_obj_set_style_bg_color(btn_list, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn_list, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_list, 3, 0);
    lv_obj_set_style_border_color(btn_list, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(btn_list, 0, 0);
    lv_obj_t *list_label = lv_label_create(btn_list);
    lv_obj_set_style_text_font(list_label, &ui_cjk_40, 0);
    lv_obj_set_style_text_color(list_label, lv_color_black(), 0);
    lv_label_set_text(list_label, "歌单");
    lv_obj_center(list_label);

    return scr;
}

void MusicPage::on_btn_clicked(lv_event_t *e) {
    auto *ctx = static_cast<ButtonCtx *>(lv_event_get_user_data(e));
    if (ctx && ctx->page) {
        ctx->page->emit(ctx->action);
    }
}
void MusicPage::emit(MusicPageAction action) {
    if (cb_) {
        cb_(action, user_data_);
    }
}

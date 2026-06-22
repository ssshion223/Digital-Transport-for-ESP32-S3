#include "lvgl.h"
#include "folder_page.h"
LV_FONT_DECLARE(song_cjk_16);
LV_FONT_DECLARE(ui_cjk_40);

FolderPage::FolderPage() {}
FolderPage::~FolderPage() {}

lv_obj_t *FolderPage::create(const std::vector<ScanItem> &items, 
FolderPageActionCallBack cb, void *user_data) {
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    items_ = items;
    cb_ = cb;
    user_data_ = user_data;
    size_t visible_count = items_.size();
    button_ctxs_.clear();
    button_ctxs_.resize(visible_count);

    lv_obj_t *list = lv_obj_create(scr);
    lv_obj_set_size(list, 300, 420);
    lv_obj_set_pos(list, 10, 50);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(list, 8, 0);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    for (size_t i = 0; i < visible_count; i++) {
        const ScanItem &item = items_[i];
        FolderPageAction action = item.is_dir 
        ? FolderPageAction::OPEN_FOLDER 
        : FolderPageAction::OPEN_MUSIC_FILE;
        button_ctxs_[i] = {action, this, i};

        lv_obj_t *btn = lv_button_create(list);
        lv_obj_add_event_cb(btn, on_btn_clicked, LV_EVENT_CLICKED, &button_ctxs_[i]);
        lv_obj_set_height(btn, 44);
        lv_obj_set_style_radius(btn, 0, 0);
        lv_obj_set_style_shadow_width(btn, 0, 0);
        lv_obj_set_style_bg_color(btn, lv_color_white(), 0);
        lv_obj_set_style_border_width(btn, 2, 0);
        lv_obj_set_style_border_color(btn, lv_color_black(), 0);
        lv_obj_set_width(btn, LV_PCT(100));
        lv_obj_t *label = lv_label_create(btn);
        lv_obj_set_style_text_font(label, &song_cjk_16, 0);
        lv_obj_set_style_text_color(label, lv_color_black(), 0);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(label, LV_PCT(100));
        std::string text;
        if (item.is_dir) {
            text = "[DIR] " + item.name + ">";
        } else {
            text = "[FILE] " + item.name;
        }
        lv_label_set_text(label, text.c_str());
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    }

    lv_obj_t *back_btn = lv_button_create(scr);
    lv_obj_add_event_cb(back_btn, on_btn_clicked, LV_EVENT_CLICKED, &back_btn_ctx_);
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
    lv_label_set_text(page_label, "文件夹");
    lv_obj_set_style_text_font(page_label, &ui_cjk_40, 0);
    lv_obj_set_pos(page_label, 50, 0);

    lv_obj_t *divider = lv_obj_create(scr);
    lv_obj_remove_style_all(divider);
    lv_obj_set_size(divider, 320, 3);
    lv_obj_set_pos(divider, 0, 41);
    lv_obj_set_style_bg_color(divider, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(divider, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *retreat_btn = lv_button_create(scr);
    lv_obj_add_event_cb(retreat_btn, on_btn_clicked, LV_EVENT_CLICKED, &retreat_btn_ctx_);
    lv_obj_set_size(retreat_btn, 50, 40);
    lv_obj_set_pos(retreat_btn, 320-1-50, 0);
    lv_obj_set_style_shadow_width(retreat_btn, 0, 0);
    lv_obj_set_style_radius(retreat_btn, 0, 0);
    lv_obj_set_style_bg_color(retreat_btn, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(retreat_btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(retreat_btn, 0, 0);
    lv_obj_t *retreat_label = lv_label_create(retreat_btn);
    lv_obj_set_style_text_font(retreat_label, &song_cjk_16, 0);
    lv_obj_set_style_text_color(retreat_label, lv_color_black(), 0);
    lv_label_set_text(retreat_label, "后退");
    lv_obj_center(retreat_label);
    return scr;
}

void FolderPage::on_btn_clicked(lv_event_t *e) {
    auto *ctx = static_cast<ButtonCtx *>(lv_event_get_user_data(e));
    if (ctx && ctx->page) {
        ctx->page->emit(ctx->action, ctx->index);
    }
}
void FolderPage::emit(FolderPageAction action, size_t index) {
    if (!cb_) return;
    if (index >= items_.size()) return;
    cb_(action, index, user_data_);

}
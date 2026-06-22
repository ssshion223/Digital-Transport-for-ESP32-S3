#pragma once
#include "lvgl.h"
#include "scan_item.h"
#include <string>
#include <vector>
enum class FolderPageAction {
    BACK,
    RETREAT,
    OPEN_MUSIC_FILE,
    OPEN_FOLDER,
};
class FolderPage {
public:
    using FolderPageActionCallBack = void (*)(FolderPageAction action, size_t index, void *user_data);
    FolderPage();
    ~FolderPage();
    lv_obj_t *create(const std::vector<ScanItem> &items, 
        FolderPageActionCallBack cb, void *user_data);
private:
    struct ButtonCtx {
        FolderPageAction action;
        FolderPage *page;
        size_t index;
    };
    static void on_btn_clicked(lv_event_t *e);
    void emit(FolderPageAction action, size_t index);
    std::vector<ScanItem> items_;
    std::vector<ButtonCtx> button_ctxs_;
    FolderPageActionCallBack cb_ = nullptr;
    ButtonCtx back_btn_ctx_{FolderPageAction::BACK, this, 0};
    ButtonCtx retreat_btn_ctx_{FolderPageAction::RETREAT, this, 0};
    void *user_data_ = nullptr;
};
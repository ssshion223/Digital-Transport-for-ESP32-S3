#pragma once
#include "lvgl.h"
enum class MusicPageAction{
    ALBUM,
    FOLDER,
    MUSICLIST,
    PLAYING,
    BACK,
};
class MusicPage {
public:
    using MusicPageActionCallBack = void (*)(MusicPageAction action, void *user_data);
    MusicPage();
    ~MusicPage();

    lv_obj_t *create(MusicPageActionCallBack cb, void *user_data);
private:
    struct ButtonCtx {
        MusicPageAction action;
        MusicPage *page;
    };
    static void on_btn_clicked(lv_event_t *e);
    void emit(MusicPageAction action);
    MusicPageActionCallBack cb_ = nullptr;
    void *user_data_ = nullptr;
    ButtonCtx back_ctx_{MusicPageAction::BACK, this};
    ButtonCtx folder_ctx_{MusicPageAction::FOLDER, this};
    ButtonCtx playing_ctx_{MusicPageAction::PLAYING, this};
};

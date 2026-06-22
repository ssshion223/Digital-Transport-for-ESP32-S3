#pragma once
#include "lvgl.h"
#include <string>

enum class PlayPageAction {
    BACK,
    PAUSE,
};
class PlayPage {
public:
    using PlayPageActionCallBack = void (*)(PlayPageAction action, void *user_data);
    PlayPage();
    ~PlayPage();
    lv_obj_t *create(const std::string &song_name, bool paused, PlayPageActionCallBack cb, void *user_data);
    void set_paused(bool paused);
private:     
    struct ButtonCtx {
        PlayPageAction action;
        PlayPage *page;
    };
    static void on_btn_clicked(lv_event_t *e);
    void emit(PlayPageAction action);
    PlayPageActionCallBack cb_ = nullptr;
    void *user_data_ = nullptr;
    lv_obj_t *pause_label_ = nullptr;
    ButtonCtx back_ctx_{PlayPageAction::BACK, this};
    ButtonCtx pause_ctx_{PlayPageAction::PAUSE, this};
    bool paused_ = false;
};

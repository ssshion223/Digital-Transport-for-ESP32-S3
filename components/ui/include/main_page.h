#pragma once
#include "lvgl.h"
enum class MainPageAction {
    MUSIC,
    SETTING,
    SCAN,
    BUTTON2,
};
class MainPage {
public:
    using MainPageActionCallBack = void (*)(MainPageAction action, void *user_data);
    MainPage();
    ~MainPage();
    lv_obj_t *create(MainPageActionCallBack cb, void *user_data);
private:
    struct ButtonCtx {
        MainPageAction action;
        MainPage *page;
    };
    static void on_btn_clikcked(lv_event_t *e);
    void emit(MainPageAction action);
    MainPageActionCallBack cb_ = nullptr;
    void *user_data_ = nullptr;
    ButtonCtx music_ctx_{MainPageAction::MUSIC, this};
    ButtonCtx setting_ctx_{MainPageAction::SETTING, this};
    ButtonCtx scan_ctx_{MainPageAction::SCAN, this};
    ButtonCtx btn2_ctx_{MainPageAction::BUTTON2, this};
};
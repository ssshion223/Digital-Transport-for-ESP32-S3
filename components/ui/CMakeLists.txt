idf_component_register(
    SRCS "music_page.cpp" "ui.cpp" "main_page.cpp" "song_cjk_16.c" "ui_cjk_40.c" 
    "folder_page/folder_page.cpp" "play_page/play_page.cpp"
    INCLUDE_DIRS "include" "folder_page" "play_page"
    PRIV_REQUIRES driver _lvgl lvgl sd digital_decoder
    )

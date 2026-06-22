idf_component_register(
                    SRCS "lv_port_indev.c" "lv_port_fs.c" "lv_port_disp.c" "lv_port.cpp"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES driver lvgl lcd esp_timer touch
                    )

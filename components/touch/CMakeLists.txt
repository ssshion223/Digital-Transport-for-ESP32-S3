idf_component_register(
    SRCS "touch.cpp" "__I2C_TOUCH.c" "touch_port.cpp"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES esp_driver_gpio esp_driver_i2c 
    )

# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(sample_project)
set_source_files_properties(
    ${IDF_PATH}/components/esp_lcd/rgb/esp_lcd_panel_rgb.c
    PROPERTIES COMPILE_FLAGS "-O0"
)
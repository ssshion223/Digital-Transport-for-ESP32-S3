#pragma once
#include <cstdint>



namespace color {
    //PEN
    constexpr uint16_t WHITE     = 0xFFFF;
    constexpr uint16_t BLACK     = 0x0000;
    constexpr uint16_t BLUE      = 0x001F;
    constexpr uint16_t BRED      = 0XF81F;
    constexpr uint16_t GRED      = 0XFFE0;
    constexpr uint16_t GBLUE     = 0X07FF;
    constexpr uint16_t RED       = 0xF800;
    constexpr uint16_t MAGENTA   = 0xF81F;
    constexpr uint16_t GREEN     = 0x07E0;
    constexpr uint16_t CYAN      = 0x7FFF;
    constexpr uint16_t YELLOW    = 0xFFE0;
    constexpr uint16_t BROWN     = 0XBC40; //棕色
    constexpr uint16_t BRRED     = 0XFC07; //棕红色
    constexpr uint16_t GRAY      = 0X8430; //灰色
    //PANEL
    constexpr uint16_t DARKBLUE  = 0X01CF;	//深蓝色
    constexpr uint16_t LIGHTBLUE = 0X7D7C;	//浅蓝色  
    constexpr uint16_t GRAYBLUE  = 0X5458; //灰蓝色
    //
    constexpr uint16_t LIGHTGREEN= 0X841F; //浅绿色 
    constexpr uint16_t LGRAY     = 0XC618; //浅灰色(PANNEL),窗体背景色
    constexpr uint16_t LGRAYBLUE = 0XA651; //浅灰蓝色(中间层颜色)
    constexpr uint16_t LBBLUE    = 0X2B12; //浅棕蓝色(选择条目的反色)  														
}

class LCD {
public:
    LCD();
    ~LCD();
    // lcd_show_example();
private:
    uint16_t width_;
    uint16_t height_;
    uint16_t id_;
    uint16_t dir_;
    uint16_t wramcmd_;
    uint16_t setxcmd_;
    uint16_t setycmd_;

    uint16_t POINT_COLOR_;
    uint16_t BACK_COLOR_;
    void write_cmd(uint8_t cmd);
    void write_data(uint8_t data);
    void write_data16(uint16_t data16);
    void write_datan(const uint8_t *data, size_t len);
    void hard_rst();
    void clear(uint16_t color);
    void display_dir(uint8_t dir);
    void set_cursor(uint16_t x, uint16_t y);  
    void fast_draw_point(uint16_t x, uint16_t y, uint16_t color);
    void show_char(uint16_t x, uint16_t y, char c, uint8_t size, uint8_t mode);
    void set_window(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h);
    
public:
    void draw_bitmap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t *data);
};
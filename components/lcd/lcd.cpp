#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <cstdint>
#include "esp_err.h"
#include <stdio.h>

#define LCD_DC_IO      GPIO_NUM_21
#define LCD_RST_IO     GPIO_NUM_20
#define LCD_BL_IO      GPIO_NUM_45
#define LCD_RST(x)     gpio_set_level(LCD_RST_IO, x)
#define LCD_DC(x)      gpio_set_level(LCD_DC_IO, x)
#define LCD_BL(x)       gpio_set_level(LCD_BL_IO, x)
#define delay_ms(x)    vTaskDelay(pdMS_TO_TICKS(x))
#ifdef __cplusplus
extern "C" {
#endif
#include "__SPI_LCD.h"
#include "FONT.H"
#ifdef __cplusplus
}
#endif
static esp_err_t gpio_init() {
    gpio_config_t gpio_struct1{};
    gpio_struct1.intr_type = GPIO_INTR_DISABLE;
    gpio_struct1.mode = GPIO_MODE_OUTPUT;
    gpio_struct1.pin_bit_mask = 1ull << LCD_DC_IO;
    gpio_struct1.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_struct1.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&gpio_struct1);
    gpio_config_t gpio_struct2{};
    gpio_struct2.intr_type = GPIO_INTR_DISABLE;
    gpio_struct2.mode = GPIO_MODE_OUTPUT;
    gpio_struct2.pin_bit_mask = 1ull << LCD_RST_IO;
    gpio_struct2.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_struct2.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&gpio_struct2);
    gpio_config_t gpio_struct3{};
    gpio_struct3.intr_type = GPIO_INTR_DISABLE;
    gpio_struct3.mode = GPIO_MODE_OUTPUT;
    gpio_struct3.pin_bit_mask = 1ull << LCD_BL_IO;
    gpio_struct3.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_struct3.pull_up_en = GPIO_PULLUP_DISABLE;
    esp_err_t err = gpio_config(&gpio_struct3);
    return err;
}
LCD::LCD() {
    if (SPI_Init_LCD() == ESP_OK) {
        printf("spi initialization successful\r\n");
    } else {
        printf("spi initialization failed\r\n");
    }
    if (gpio_init() == ESP_OK) {
        printf("lcd io initialization successful\r\n");
    } else {
        printf("lcd io initialization failed\r\n");
    }
    LCD_BL(0);

    hard_rst();
    //************* Start Initial Sequence **********//
    // delay_ms(150); // Delay 120ms
    write_cmd(0x11); // Sleep Out
    delay_ms(120); // Delay 120ms
    write_cmd(0xf0) ;
    write_data(0xc3) ;
    write_cmd(0xf0) ;
    write_data(0x96) ;
    write_cmd(0x36);
    write_data(0x48); 
    write_cmd(0x3A);
    write_data(0x55);
    write_cmd(0xB4);
    write_data(0x01);
    write_cmd(0xB7) ;
    write_data(0xC6) ;
    write_cmd(0xe8);
    write_data(0x40);
    write_data(0x8a);
    write_data(0x00);
    write_data(0x00);
    write_data(0x29);
    write_data(0x19);
    write_data(0xa5);
    write_data(0x33);
    write_cmd(0xc1);
    write_data(0x06);
    write_cmd(0xc2);
    write_data(0xa7);
    write_cmd(0xc5);
    write_data(0x18);
    write_cmd(0xe0); //Positive Voltage Gamma Control
    write_data(0xf0);
    write_data(0x09);
    write_data(0x0b);
    write_data(0x06);
    write_data(0x04);
    write_data(0x15);
    write_data(0x2f);
    write_data(0x54);
    write_data(0x42);
    write_data(0x3c);
    write_data(0x17);
    write_data(0x14);
    write_data(0x18);
    write_data(0x1b);
    write_cmd(0xe1); //Negative Voltage Gamma Control
    write_data(0xf0);
    write_data(0x09);
    write_data(0x0b);
    write_data(0x06);
    write_data(0x04);
    write_data(0x03);
    write_data(0x2d);
    write_data(0x43);
    write_data(0x42);
    write_data(0x3b);
    write_data(0x16);
    write_data(0x14);
    write_data(0x17);
    write_data(0x1b);
    write_cmd(0xf0);
    write_data(0x3c);
    write_cmd(0xf0);
    write_data(0x69);
    write_cmd(0x21);
    delay_ms(120); //Delay 120ms
    write_cmd(0x29);// Display on
    LCD_BL(1);
    display_dir(0);
    // clear(color::WHITE);
    // POINT_COLOR_ = color::BLACK;
    // BACK_COLOR_ = color::RED;
    // delay_ms(100);
    // show_char(2, 3, 'A', 24, 1);
    // show_char(2, 3+24, 'B', 24, 1);
    // show_char(2, 3+48, 'C', 24, 1);
    // show_char(2, 3+72, 'D', 24, 1);

}

LCD::~LCD() {}

void LCD::write_cmd(uint8_t cmd) { // write reg
    LCD_DC(0);
    spi_lcd_transfer_data(&cmd, 1);
}

void LCD::write_data(uint8_t data) {
    LCD_DC(1);
    spi_lcd_transfer_data(&data, 1);
}

void LCD::write_data16(uint16_t data16) {
    uint8_t buffer[2];
    buffer[0] = data16 >> 8;
    buffer[1] = data16 & 0xFF;
    LCD_DC(1);
    spi_lcd_transfer_data(buffer, 2);
}
void LCD::write_datan(const uint8_t *data, size_t len) {
    LCD_DC(1);
    spi_lcd_transfer_data(data, len);
}
void LCD::hard_rst() {
    LCD_RST(0);
	delay_ms(50);
	LCD_RST(1);
	delay_ms(150);
}
void LCD::clear(uint16_t color) {
    uint32_t total = this->width_ * this->height_;
    set_cursor(0x00, 0x0000);
    write_cmd(this->wramcmd_);
    for (uint32_t i = 0; i < total; i++ ) {
        write_data16(color);
    }
}
void LCD::set_cursor(uint16_t x, uint16_t y) {
    write_cmd(this->setxcmd_); 
    write_data16(x);
    write_data16(this->width_-1); 
    write_cmd(this->setycmd_); 
    write_data16(y);
    write_data16(this->height_-1); 		
}
void LCD::display_dir(uint8_t dir) {
//dir:方向选择 	0-0度旋转，1-180度旋转，2-270度旋转，3-90度旋转
	if (dir == 0 || dir == 1) {			//竖屏
        this->dir_ = 0;	//竖屏
        this->width_ = 320;
        this->height_ = 480;
        this->wramcmd_ = 0X2C;
        this->setxcmd_ = 0X2A;
        this->setycmd_ = 0X2B;
        if (dir == 0) {    //0-0度旋转
            write_cmd(0x36); 
            write_data((1<<3)|(0<<7)|(1<<6)|(0<<5));
        } else {					//1-180度旋转
            write_cmd(0x36); 
            write_data((1<<3)|(1<<7)|(0<<6)|(0<<5));		
        }
	} else if (dir == 2 || dir == 3) {
        this->dir_ = 1;	//横屏
        this->width_ = 480;
        this->height_ = 320;
        this->wramcmd_ = 0X2C;
        this->setxcmd_ = 0X2A;
        this->setycmd_ = 0X2B; 
        if (dir == 2) {			//2-270度旋转
            write_cmd(0x36); 
            write_data((1<<3)|(1<<7)|(1<<6)|(1<<5));
        } else {							//3-90度旋转
            write_cmd(0x36); 
            write_data((1<<3)|(0<<7)|(0<<6)|(1<<5));
        }		
	}	
		//设置显示区域	
		write_cmd(this->setxcmd_); 
		write_data16(0x0000);
		write_data16(this->width_-1);
		write_cmd(this->setycmd_); 
		write_data16(0x0000);
		write_data16(this->height_-1);  
}


void LCD::fast_draw_point(uint16_t x, uint16_t y, uint16_t color) {
    set_cursor(x, y);
    write_cmd(this->wramcmd_);
    write_data16(color);
}

void LCD::show_char(uint16_t x, uint16_t y, char c, uint8_t size, uint8_t mode) {
    uint8_t temp, t1, t;
	uint16_t y0 = y;
	uint8_t csize = (size / 8+ ((size%8) ? 1 : 0 ))*(size / 2);		//得到字体一个字符对应点阵集所占的字节数	
 	uint8_t num = c - ' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t = 0; t < csize; t++)
	{   
		if (size == 12) temp = asc2_1206[num][t]; 	 	//调用1206字体
		else if (size == 16) temp = asc2_1608[num][t];	//调用1608字体
		else if (size == 24) temp = asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1 = 0; t1 < 8; t1++)
		{			    
			if (temp&0x80) fast_draw_point(x,y,POINT_COLOR_);
			else if (mode==0) fast_draw_point(x,y,BACK_COLOR_);
			temp<<=1;
			y++;
			if (y >= height_) return;		//超区域了
			if ((y - y0) == size)
			{
				y = y0;
				x++;
				if (x >= width_) return;	//超区域了
				break;
			}
		}  	 
	}  	    
}
void LCD::set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    write_cmd(setxcmd_);
    write_data16(x1);
    write_data16(x2);
    write_cmd(setycmd_);
    write_data16(y1);
    write_data16(y2);
}

void LCD::draw_bitmap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t *data) {
    set_window(x1, y1, x2, y2);
    uint16_t w = x2 - x1 +1;
    uint16_t h = y2 - y1 +1;
    size_t len = w * h * 2;
    write_cmd(wramcmd_);
    write_datan(data, len);
}

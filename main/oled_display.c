#include "oled_display.h"
#include "ssd1306.h"
#include "font8x8_basic.h"
#include <stdio.h>

static i2c_master_dev_handle_t i2c_handle;
static ssd1306_t oled_dev;

void oled_init(void) {
    i2c_master_bus_config_t i2c_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_6,
        .scl_io_num = GPIO_NUM_7,
        .glitch_ignore_cnt = 7,
    };

    i2c_master_bus_handle_t bus_handle;
    i2c_master_bus_init(I2C_NUM_0, &i2c_config, &bus_handle);

    ssd1306_config_t oled_config = {
        .dev = &oled_dev,
        .bus = bus_handle,
        .panel = SSD1306_PANEL_I2C,
        .addr = 0x3C
    };

    ssd1306_init(&oled_config);
    ssd1306_clear_screen(&oled_dev);
    ssd1306_contrast(&oled_dev, 0xFF);
}

void oled_display_readings(float temperature, float humidity, bool is_alert) {
    char temp_buf[32];
    char hum_buf[32];
    char alert_buf[16];

    snprintf(temp_buf, sizeof(temp_buf), "Temp: %.1f C", temperature);
    snprintf(hum_buf, sizeof(hum_buf), "Hum : %.1f %%", humidity);
    snprintf(alert_buf, sizeof(alert_buf), "ALERT!");

    ssd1306_clear_screen(&oled_dev);
    ssd1306_display_text(&oled_dev, 0, temp_buf, strlen(temp_buf), false);
    ssd1306_display_text(&oled_dev, 1, hum_buf, strlen(hum_buf), false);
    if (is_alert) {
        ssd1306_display_text(&oled_dev, 2, alert_buf, strlen(alert_buf), false);
    }
}

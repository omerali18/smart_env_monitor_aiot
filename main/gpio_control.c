// gpio_control.c
#include "driver/gpio.h"
#include "gpio_control.h"

#define BUZZER_GPIO     4
#define BUTTON_GPIO     5

void gpio_control_init(void) {
    gpio_config_t buzzer_config = {
        .pin_bit_mask = 1ULL << BUZZER_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&buzzer_config);
    gpio_set_level(BUZZER_GPIO, 0); // Initially off

    gpio_config_t button_config = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&button_config);
}

void buzzer_on(void) {
    gpio_set_level(BUZZER_GPIO, 1);
}

void buzzer_off(void) {
    gpio_set_level(BUZZER_GPIO, 0);
}

bool is_button_pressed(void) {
    return gpio_get_level(BUTTON_GPIO) == 0; // active LOW
}

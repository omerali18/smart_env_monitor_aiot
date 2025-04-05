#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void oled_init(void);
void oled_display_readings(float temperature, float humidity, bool is_alert);

#ifdef __cplusplus
}
#endif

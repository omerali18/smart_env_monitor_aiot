// sensor_task.c
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "dht.h"
#include "esp_log.h"
#include "app_wifi.h"
#include "oled_display.h"
#include "sensor_task.h"
#include "iot_button.h"
#include "iot_button_v2.h"
#include "esp_rmaker_standard_params.h"
#include "esp_rmaker_core.h"

#define DHT_GPIO        2
#define TAG             "SENSOR_TASK"

extern esp_rmaker_param_t *temperature_param;
extern esp_rmaker_param_t *humidity_param;
extern esp_rmaker_param_t *alert_param;

static float temperature = 0;
static float humidity = 0;
static bool alert_state = false;

static void sensor_loop(void *arg) {
    while (1) {
        int16_t temp = 0, hum = 0;

        if (dht_read_data(DHT_TYPE_DHT11, DHT_GPIO, &hum, &temp) == ESP_OK) {
            temperature = temp / 10.0;
            humidity = hum / 10.0;
            ESP_LOGI(TAG, "Temperature: %.1f°C | Humidity: %.1f%%", temperature, humidity);

            esp_rmaker_param_update_and_report(temperature_param, esp_rmaker_float(temperature));
            esp_rmaker_param_update_and_report(humidity_param, esp_rmaker_float(humidity));
            
            if (temperature > 30.0 || humidity > 70.0) {
                if (!alert_state) {
                    alert_state = true;
                    oled_display_alert("Alert: High Temp/Humidity!");
                    esp_rmaker_param_update_and_report(alert_param, esp_rmaker_bool(true));
                }
            } else {
                if (alert_state) {
                    alert_state = false;
                    oled_display_alert("Environment Normal");
                    esp_rmaker_param_update_and_report(alert_param, esp_rmaker_bool(false));
                }
            }

            oled_display_data(temperature, humidity, alert_state);
        } else {
            ESP_LOGW(TAG, "DHT read failed.");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void sensor_task_start(void) {
    xTaskCreate(sensor_loop, "sensor_task", 4096, NULL, 5, NULL);
}

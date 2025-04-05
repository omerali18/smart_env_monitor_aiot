#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "dht.h"
#include "driver/gpio.h"
#include "ssd1306.h"

#include "esp_rmaker_core.h"
#include "esp_rmaker_standard_devices.h"
#include "esp_rmaker_standard_params.h"
#include "esp_rmaker_ota.h"
#include "app_wifi.h"

#define TAG "APP_MAIN"

#define DHT11_GPIO        2
#define BUZZER_GPIO       4
#define PUSH_BUTTON_GPIO  5

// RainMaker Devices
static TemperatureSensorDevice *temp_device = NULL;
static TemperatureSensorDevice *humidity_device = NULL;
static SwitchDevice *alert_device = NULL;

// Simulated thresholds
#define TEMP_THRESHOLD    30.0
#define HUM_THRESHOLD     70.0

static void check_anomaly(float temp, float hum) {
    bool alert = (temp > TEMP_THRESHOLD || hum > HUM_THRESHOLD);

    gpio_set_level(BUZZER_GPIO, alert ? 1 : 0);
    esp_rmaker_param_update_and_report(alert_device->state, esp_rmaker_bool(alert));
    ssd1306_clear_screen();
    ssd1306_display_text("Temp: %.1fC", temp);
    ssd1306_display_text("Hum : %.1f%%", hum);
    ssd1306_display_text("Alert: %s", alert ? "YES" : "NO");
}

static void sensor_task(void *arg) {
    float temp = 0.0f, hum = 0.0f;

    while (1) {
        esp_err_t result = dht_read_data(DHT_TYPE_DHT11, DHT11_GPIO, &hum, &temp);
        if (result == ESP_OK) {
            ESP_LOGI(TAG, "Temp: %.1f C | Hum: %.1f %%", temp, hum);
            esp_rmaker_param_update_and_report(temp_device->temperature, esp_rmaker_float(temp));
            esp_rmaker_param_update_and_report(humidity_device->temperature, esp_rmaker_float(hum));
            check_anomaly(temp, hum);
        } else {
            ESP_LOGW(TAG, "DHT read failed");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main(void) {
    // Initialize NVS
    ESP_ERROR_CHECK(nvs_flash_init());

    // Init GPIO
    gpio_pad_select_gpio(BUZZER_GPIO);
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(BUZZER_GPIO, 0);

    gpio_pad_select_gpio(PUSH_BUTTON_GPIO);
    gpio_set_direction(PUSH_BUTTON_GPIO, GPIO_MODE_INPUT);

    // Init OLED
    ssd1306_init();

    // Initialize Wi-Fi
    app_wifi_init();
    app_wifi_start();

    // Create Node
    esp_rmaker_node_t *node = esp_rmaker_node_init_default("Smart Env Monitor", "SmartDevice", "v1.0");

    // Devices
    temp_device = esp_rmaker_temp_sensor_device_create("Temperature", NULL, 25.0);
    humidity_device = esp_rmaker_temp_sensor_device_create("Humidity", NULL, 50.0);
    alert_device = esp_rmaker_switch_device_create("Alert", NULL, false);

    esp_rmaker_node_add_device(node, temp_device);
    esp_rmaker_node_add_device(node, humidity_device);
    esp_rmaker_node_add_device(node, alert_device);

    // OTA + Schedule + Timezone
    esp_rmaker_ota_enable(OTA_USING_PARAMS);
    esp_rmaker_timezone_service_enable();
    esp_rmaker_schedule_enable();

    esp_rmaker_start();

    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);
}

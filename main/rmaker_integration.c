// rmaker_integration.c
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_ota.h>
#include <esp_rmaker_schedule.h>
#include <esp_rmaker_time.h>
#include <esp_rmaker_common_events.h>
#include "rmaker_integration.h"
#include "gpio_control.h"

static esp_rmaker_node_t *node = NULL;
static esp_rmaker_device_t *switch_device = NULL;
static esp_rmaker_device_t *alert_device = NULL;
static esp_rmaker_param_t *switch_param = NULL;

static void write_callback(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
                           const esp_rmaker_param_val_t val, void *priv_data) {
    const char *param_name = esp_rmaker_param_get_name(param);

    if (strcmp(param_name, ESP_RMAKER_DEF_POWER_NAME) == 0) {
        bool power = val.val.b;
        esp_rmaker_param_update_and_report(param, val);

        if (power) {
            buzzer_on();
        } else {
            buzzer_off();
        }
    }
}

void rmaker_node_init(void) {
    node = esp_rmaker_node_init_default("Smart Env Monitor", "AIoT.Device");
    if (!node) {
        printf("Could not initialize node.\n");
        return;
    }

    // Switch device (buzzer)
    switch_device = esp_rmaker_device_create("Buzzer Switch", ESP_RMAKER_DEVICE_SWITCH, NULL);
    switch_param = esp_rmaker_param_create(ESP_RMAKER_DEF_POWER_NAME, ESP_RMAKER_PARAM_POWER,
                                           esp_rmaker_bool(false), PROP_FLAG_READ | PROP_FLAG_WRITE);
    esp_rmaker_device_add_param(switch_device, switch_param);
    esp_rmaker_device_assign_primary_param(switch_device, switch_param);
    esp_rmaker_device_add_cb(switch_device, write_callback, NULL);
    esp_rmaker_node_add_device(node, switch_device);

    // Alert status (read-only)
    alert_device = esp_rmaker_device_create("Alert Status", NULL, NULL);
    esp_rmaker_device_add_param(alert_device,
        esp_rmaker_param_create("Alert", NULL,
            esp_rmaker_str("NO"), PROP_FLAG_READ));
    esp_rmaker_node_add_device(node, alert_device);

    esp_rmaker_ota_enable_default();
    esp_rmaker_timezone_service_enable();
    esp_rmaker_schedule_enable();
    esp_rmaker_start();
}

void rmaker_update_params(float temperature, float humidity, bool alert) {
    // Update alert param
    esp_rmaker_param_t *alert_param = esp_rmaker_device_get_param_by_name(alert_device, "Alert");
    if (alert_param) {
        esp_rmaker_param_update_and_report(alert_param,
            esp_rmaker_str(alert ? "YES" : "NO"));
    }
}

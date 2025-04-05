#include "app_wifi.h"
#include "esp_log.h"
#include "esp_rmaker_core.h"
#include "esp_rmaker_standard_types.h"
#include "esp_rmaker_standard_params.h"
#include "app_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "wifi_provisioning/manager.h"

static const char *TAG = "app_wifi";

void wifi_event_handler(void* arg, esp_event_base_t event_base,
                        int32_t event_id, void* event_data) {
    if (event_base == WIFI_PROV_EVENT && event_id == WIFI_PROV_START) {
        ESP_LOGI(TAG, "Provisioning started");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Connected to Wi-Fi");
    }
}

void app_wifi_start_provisioning(const char *pop, const char *service_name) {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    wifi_prov_mgr_config_t config = {
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM,
        .app_info = NULL
    };
    wifi_prov_mgr_init(config);

    bool provisioned = false;
    wifi_prov_mgr_is_provisioned(&provisioned);
    if (!provisioned) {
        ESP_LOGI(TAG, "Starting BLE provisioning");
        wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_1, pop, service_name);
    } else {
        ESP_LOGI(TAG, "Already provisioned, starting Wi-Fi...");
        wifi_prov_mgr_deinit();
    }
}

#include "app_flash.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

static const char *TAG = "App_Flash";

esp_err_t save_device_state(const uint8_t* data, size_t length) {
    nvs_handle_t handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    err = nvs_set_blob(handle, KEY_DEVICE_STATE, data, length);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error writing state: %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error committing changes: %s", esp_err_to_name(err));
    }

    nvs_close(handle);
    return err;
}

esp_err_t read_device_state(uint8_t* data, size_t* length) {
    nvs_handle_t handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    err = nvs_get_blob(handle, KEY_DEVICE_STATE, NULL, length);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error getting blob size: %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }
    
    err = nvs_get_blob(handle, KEY_DEVICE_STATE, data, length);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error reading state: %s", esp_err_to_name(err));
    }

    nvs_close(handle);
    return err;
}
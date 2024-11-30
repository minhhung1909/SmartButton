// app_flash.h
#ifndef APP_FLASH_H
#define APP_FLASH_H

#include <stdint.h>
#include "esp_err.h"

#define STORAGE_NAMESPACE "storage"
#define KEY_DEVICE_STATE "dev_state"

esp_err_t save_device_state(const uint8_t* data, size_t length);
esp_err_t read_device_state(uint8_t* data, size_t* length);

#endif
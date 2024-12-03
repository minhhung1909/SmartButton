set(app_src "app_tcp_client.c" "app_config.c" "frame_crc.c" "control_device.c" "app_flash.c" "app_OTA.c")
set(pri_req)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES driver esp_wifi nvs_flash button esp_event app_update esp_http_client mbedtls esp_https_ota ${pri_req})


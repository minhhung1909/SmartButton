set(app_src "app_tcp_client.c" "app_config.c" "frame_crc.c" "control_device.c" "app_flash.c")
set(pri_req)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES driver esp_wifi nvs_flash button ${pri_req})


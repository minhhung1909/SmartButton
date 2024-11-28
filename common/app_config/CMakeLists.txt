set(app_src "app_config.c")
set(pri_req)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES driver esp_wifi nvs_flash ${pri_req})


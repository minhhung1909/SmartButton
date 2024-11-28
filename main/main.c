#include <stdio.h>
#include "nvs_flash.h"
#include "app_config.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_smartconfig.h"
#include "freertos/event_groups.h"
#include "button_gpio.h"
#include "iot_button.h"
#include "frame_crc.h"

// #include "esp_eap_client.h"

void on_press(void* arg);
void init_Button();

static const char *TAG_MAIN = "MAIN";

void app_main(void){
    init_Button();

    ESP_LOGI(TAG_MAIN, "Starting app_main");
    ESP_LOGI(TAG_MAIN, "Free memory %d byte", (int)esp_get_free_heap_size());
    
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    app_config(CHANGE_WIFI_OFF);


    /* START CASE CHAGE WIFI WHEN NEED */
    // clear_wifi_credentials();
    // ESP_ERROR_CHECK(esp_wifi_stop());
    // ESP_ERROR_CHECK(esp_wifi_deinit());
    // ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(esp_netif_init());
    // app_config(CHANGE_WIFI_ON);
    /* END CASE CHAGE WIFI WHEN NEED */

}


void on_press(void* arg) {
    printf("Button pressed!\n");
}

void init_Button() {
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = 5000,
        .gpio_button_config = {
            .gpio_num = GPIO_NUM_45,
            .active_level = 1,
        },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if (NULL == gpio_btn) {
        printf("Button create failed!\n");
        return;
    }
    iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_START, on_press, NULL);
}
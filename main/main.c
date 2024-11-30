#include <stdio.h>
#include "nvs_flash.h"
#include "app_config.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_smartconfig.h"
#include "freertos/event_groups.h"
#include "frame_crc.h"
#include "app_tcp_client.h"
#include "control_device.h"

void tcp_client_task(void *pvParameters);
void processing_Data_Task(void *pvParameters);

static const char *TAG_MAIN = "MAIN";

void app_main(void){
    /* START INIT */
    init_Button();
    init_device();

    ESP_LOGI(TAG_MAIN, "Starting app_main");
    ESP_LOGI(TAG_MAIN, "Free memory %d byte", (int)esp_get_free_heap_size());
    
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    app_config(CHANGE_WIFI_OFF);

    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
    xTaskCreate(processing_Data_Task, "processing_Data_Task", 4096, NULL, 5, NULL);

    

    /* END INIT */
    while(1){

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}



void tcp_client_task(void *pvParameters) {
    tcp_client("192.168.2.88", 2000);
    vTaskDelete(NULL);
}


void processing_Data_Task(void *pvParameters) {
    while(1){
        if (g_size_data > 0) {
            control_device();            
            g_size_data = 0;
        }
        // Add delay to yield CPU
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
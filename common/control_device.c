#include "control_device.h"
#include <time.h>

static const char *TAG_CONTROL_DEVICE = "Control Device";

gpio_num_t LED = GPIO_NUM_2;
int hour, minute, second, day, month, year;

void on_press(void* arg) {
    gpio_set_level(LED, 1);
    printf("Button pressed!\n");
    /* START CASE CHAGE WIFI WHEN NEED */
    // clear_wifi_credentials();
    // ESP_ERROR_CHECK(esp_wifi_stop());
    // ESP_ERROR_CHECK(esp_wifi_deinit());
    // ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(esp_netif_init());
    // app_config(CHANGE_WIFI_ON);
    /* END CASE CHAGE WIFI WHEN NEED */
}

void on_release(){
    gpio_set_level(LED, 0);
    printf("Button released!\n");
}

int init_Button() {
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = 3000,
        .short_press_time = 100,
        .gpio_button_config = {
            .gpio_num = GPIO_NUM_45,
            .active_level = 1,
        },
    };

    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    
    if (NULL == gpio_btn) {
        printf("Button create failed!\n");
        return -1;
    }
    iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_START, on_press, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_PRESS_UP, on_release, NULL);
    return 0;
}

void init_device() {
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_INPUT_OUTPUT);
}

char get_time_full() {
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    time(&now);
    setenv("TZ", "CST-7", 1);
    tzset();

    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG_CONTROL_DEVICE, "The current date/time in Hanoi is: %s", strftime_buf);
    return strftime_buf;
}

void reset_Time(){
    hour = -1;
    minute = -1;
    second = -1;
    day = -1;
    month = -1;
    year = -1;
}

void run_set_alarm(void *pvParameters){
    hour = (int)(g_data[0]), minute = (int)(g_data[1]), second = (int)(g_data[2]), day = (int)(g_data[3]), month = (int)(g_data[4]), year = (int)(g_data[5]);
    ESP_LOGI(TAG_CONTROL_DEVICE, "Set alarm at %d:%d:%d Day: %d\n", hour, minute, second, day);
    time_t now;
    struct tm timeinfo;
    while(1){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
        if (second <= timeinfo.tm_sec &&  minute <= timeinfo.tm_min && hour == timeinfo.tm_hour && day == timeinfo.tm_mday) {
            gpio_set_level(LED, 1);
            reset_Time();
            ESP_LOGI(TAG_CONTROL_DEVICE, "Alarm is running... \n");
            break;
        }
    }
    vTaskDelete(NULL);
}

void control_device() {
    int old_state_led = gpio_get_level(LED);
    if ( *(unsigned int*)g_command == TURN_ON_DEVICE ) {
        gpio_set_level(LED, 1);
        printf("Turn on device\n");
    }else if( *(unsigned int*)g_command == TURN_OFF_DEVICE ) {
        gpio_set_level(LED, 0);
        printf("Turn off device\n");
    }else if ( *(unsigned int*)g_command == GET_STATE_DEVICE ) {
        gpio_get_level(LED);
        ESP_LOGI(TAG_CONTROL_DEVICE, "State device: %d\n", gpio_get_level(LED));
    }
    else if ( *(unsigned int*)g_command == ALARM ) {
        xTaskCreate(run_set_alarm, "run_set_alarm", 4096, NULL, 5, NULL);
    }
    int new_state_led = gpio_get_level(LED);
    // if (old_state_led != new_state_led || *(unsigned int*)g_command == ALARM) {
    //     uint8_t data[] = {gpio_get_level(LED), 
    //             "%d|%d|%d|%d|%d|%d", hour, minute, second, day, month, year}; // save {state device} and {time control} (vẫn chưa làm lưu thời gian)
    //     save_device_state(data, sizeof(data));
    // }
}
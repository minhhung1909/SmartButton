#include "control_device.h"

static const char *TAG_CONTROL_DEVICE = "Control Device";

gpio_num_t LED = GPIO_NUM_2;

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

int update_time(){
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    esp_netif_sntp_init(&config);

    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
        printf("Failed to update system time within 10s timeout");
        return -1;
    }
    return 0;
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
    ESP_LOGI(TAG_CONTROL_DEVICE, "The current date/time in HoChiMinh is: %s", strftime_buf);
    return strftime_buf;
}

int get_hour(){
    if(update_time() < 0){
        return -1;
    }
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_hour;
}

int get_minute(){
    if(update_time() < 0){
        return -1;
    }
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_min;
}

int get_second(){
    if(update_time() < 0){
        return -1;
    }
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_sec;
}

int get_day(){
    if(update_time() < 0){
        return -1;
    }
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    printf("Day: %d\n", timeinfo.tm_mday);
    return timeinfo.tm_mday;
}

int get_month(){
    if(update_time() < 0){
        return -1;
    }
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    printf("Month: %d\n", timeinfo.tm_mon + 1);
    return timeinfo.tm_mon + 1;
}

int get_year() {
    if(update_time() < 0){
        return -1;
    }
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    printf("Year: %d\n", timeinfo.tm_year + 1900);
    return timeinfo.tm_year + 1900;
}

int hex_to_dec(uint8_t hex) {
    return (hex >> 4) * 10 + (hex & 0x0F);
}

void set_time_deivce(gpio_num_t Device, int State_Device){
    int hour = hex_to_dec(g_data[0]);
    int minute = hex_to_dec(g_data[1]);
    int second = hex_to_dec(g_data[2]);
    int day = hex_to_dec(g_data[3]);
    int month = hex_to_dec(g_data[4]);
    int year = hex_to_dec(g_data[5]);

    if (hour == get_hour() && minute == get_minute() && second >= get_second() && day == get_day() && month == get_month() && year == get_year()) {
        gpio_set_level(Device, State_Device);
    }
}

void control_device() {
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
        set_time_deivce(LED, ON);
        ESP_LOGI(TAG_CONTROL_DEVICE, "Alarm...........................\n");
    }
}
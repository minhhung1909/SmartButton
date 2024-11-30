#ifndef __CONTROL_DEVICE_H__
#define __CONTROL_DEVICE_H__

#define ON      1
#define OFF     0

#include "iot_button.h"
#include "command_control.h"
#include "driver/gpio.h"
#include "app_tcp_client.h"
#include "esp_log.h"
#include "esp_netif_sntp.h"
#include <string.h>
#include "freertos/event_groups.h"
#include "app_flash.h"

void control_device();
void init_device();
int init_Button();
void on_release();
void on_press();
void on_press(void* arg);
char get_time_full();

#endif // __CONTROL_DEVICE_H__
#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

typedef enum{
    CHANGE_WIFI_OFF = 0,
    CHANGE_WIFI_ON
}change_wifi_t;

typedef enum {
    PROVISION_SMARTCONFIG = 0,
    PROVISION_ACCESSPOINT
}provision_type_t;

void app_config(change_wifi_t change_wifi);
void clear_wifi_credentials(void);

#endif // __APP_CONFIG_H
#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

typedef enum {
    PROVISION_SMARTCONFIG = 0,
    PROVISION_ACCESSPOINT
}provision_type_t;

void app_config(void);

#endif // __APP_CONFIG_H
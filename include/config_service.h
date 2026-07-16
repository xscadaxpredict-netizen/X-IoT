#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H

#include <Arduino.h>
#include "system_err.h"
#include "wifi_service.h"
#include "mqtt_service.h"
#include "modbus_service.h"
#include "acquisition.h"
#include "publisher.h"
#include "nextion_service.h"

sys_status_t config_init(void);

const app_wifi_config_t* config_get_wifi(void);
const mqtt_config_t* config_get_mqtt(void);
const modbus_config_t* config_get_modbus(void);
const acquisition_config_t* config_get_acq(void);
const publisher_config_t* config_get_publisher(void);
const nextion_config_t* config_get_nextion(void);

#endif
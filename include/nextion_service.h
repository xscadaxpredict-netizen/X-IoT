#ifndef NEXTION_SERVICE_H
#define NEXTION_SERVICE_H

#include <Arduino.h>
#include "system_err.h"

typedef struct {
  uint8_t rxPin;
  uint8_t txPin;
  uint32_t baudrate;
  uint32_t intervalMs;
} nextion_config_t;

typedef enum {
  NEXTION_STATE_DEINITIALIZED = 900,
  NEXTION_STATE_INITIALIZED = 901,
  NEXTION_STATE_RUNNING = 902,
  NEXTION_STATE_STOPPED = 903
} nextion_state_t;

sys_status_t nextion_init(const nextion_config_t* cfg);
sys_status_t nextion_start(void);
sys_status_t nextion_stop(void);
nextion_state_t nextion_get_state(void);

#endif
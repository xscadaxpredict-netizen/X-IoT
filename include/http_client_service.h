#ifndef HTTP_CLIENT_SERVICE_H
#define HTTP_CLIENT_SERVICE_H

#include <Arduino.h>
#include "system_err.h"

// Define the state of the HTTP client
typedef enum {
  HTTP_CLIENT_STATE_DEINITIALIZED,
  HTTP_CLIENT_STATE_INITIALIZED,
  HTTP_CLIENT_STATE_RUNNING,
  HTTP_CLIENT_STATE_STOPPED
} http_client_state_t;

// Define what we are downloading
typedef enum {
  HTTP_ACTION_OTA_UPDATE,
  HTTP_ACTION_TAG_UPDATE
} http_action_t;

// The request structure pushed to the Queue
typedef struct {
  http_action_t action;
  char url[128]; // Ensure URLs sent via MQTT fit within this limit
} http_download_request_t;

// Core Lifecycle APIs
sys_status_t http_client_init(void);
sys_status_t http_client_start(void);
sys_status_t http_client_stop(void);
http_client_state_t http_client_get_state(void);

// Trigger a background download
sys_status_t http_client_download(http_action_t action, const char* url);

#endif
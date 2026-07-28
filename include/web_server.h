#ifndef WEB_SERVER_H
#define WEB_SERVER_H
#include "system_err.h"

typedef struct {
  const char* username;
  const char* password;
} portal_config_t;

sys_status_t web_server_init(const portal_config_t* cfg);
sys_status_t web_server_start(void);
sys_status_t web_server_stop(void);

#endif
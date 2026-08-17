#ifndef SYSTEM_SERVICE_H
#define SYSTEM_SERVICE_H

#include "system_err.h"
#include "protocol_dispatcher.h" // For protocol_request_t

#define GATEWAY_VERSION "3.4.2-dev"

sys_status_t system_service_read_request(const protocol_request_t* request);

#endif
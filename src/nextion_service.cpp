#include "nextion_service.h"
#include "tag_runtime.h"
#include "logger.h"
#include <Arduino.h>

#define MODULE "NEXTION"
#define NEXTION_SERIAL Serial2

static nextion_config_t gConfig;
static nextion_state_t gState = NEXTION_STATE_DEINITIALIZED;
static TaskHandle_t gTaskHandle = NULL;

static void sendTerminator() {
    NEXTION_SERIAL.write(0xFF);
    NEXTION_SERIAL.write(0xFF);
    NEXTION_SERIAL.write(0xFF);
}

static void sendText(const char* component, const char* text) {
    NEXTION_SERIAL.print(component);
    NEXTION_SERIAL.print(".txt=\"");
    NEXTION_SERIAL.print(text);
    NEXTION_SERIAL.print("\"");
    sendTerminator();
}

static void update_hmi_widget(const tag_runtime_t* tag) {
    if (tag == NULL || tag->config == NULL || tag->config->nxtComponent == NULL) {
        return;
    }

    char buf[64];
    tag_type_t valType = (tag->config->valueDataType == 0) ? tag->config->dataType : tag->config->valueDataType;

    switch (valType) {
        case TAG_BOOL:
            sendText(tag->config->nxtComponent, tag->value.bv ? "ON" : "OFF");
            break;

        case TAG_UINT16:
            snprintf(buf, sizeof(buf), "%u", tag->value.u16v);
            sendText(tag->config->nxtComponent, buf);
            break;

        case TAG_INT16:
            snprintf(buf, sizeof(buf), "%d", tag->value.i16v);
            sendText(tag->config->nxtComponent, buf);
            break;

        case TAG_UINT32:
            snprintf(buf, sizeof(buf), "%lu", tag->value.u32v);
            sendText(tag->config->nxtComponent, buf);
            break;

        case TAG_INT32:
            snprintf(buf, sizeof(buf), "%ld", tag->value.i32v);
            sendText(tag->config->nxtComponent, buf);
            break;

        case TAG_FLOAT32:
            snprintf(buf, sizeof(buf), "%.2f", tag->value.f32v);
            sendText(tag->config->nxtComponent, buf);
            break;

        case TAG_STRING:
            sendText(tag->config->nxtComponent, tag->value.strv);
            break;

        default:
            break;
    }
}

static void nextionTask(void* pvParameters) {
  LOG_INFO(MODULE, "Task started");
  tag_runtime_t localTag;

    while (gState == NEXTION_STATE_RUNNING) {
        for (uint16_t i = 0; i < tag_count(); i++) {
            tag_runtime_t* runtimeTag = tag_runtime_get_at(i);
            if (runtimeTag == NULL) continue;
            if (!runtimeTag->valid) continue;
            if (runtimeTag->config->nxtComponent == NULL) continue;
        
            // Thread-safe copy of the runtime value
            memset(&localTag, 0, sizeof(tag_runtime_t));
            if (tag_runtime_lock() == SYS_OK) {
                memcpy(&localTag, runtimeTag, sizeof(tag_runtime_t));
                tag_runtime_unlock();
            } 
            else {
                continue;
            }

            update_hmi_widget(&localTag);
        }
        vTaskDelay(pdMS_TO_TICKS(gConfig.intervalMs));
    }

    LOG_INFO(MODULE, "Task stopped");
    gTaskHandle = NULL;
    vTaskDelete(NULL);
}

sys_status_t nextion_init(const nextion_config_t* cfg) {
    if (gState != NEXTION_STATE_DEINITIALIZED) {
        LOG_ERROR(MODULE, "Invalid state");
        return SYS_ERR_INVALID_STATE;
    }
    if (cfg == NULL) {
        LOG_ERROR(MODULE, "Invalid params");
        return SYS_ERR_INVALID_PARAM;
    }

    memcpy(&gConfig, cfg, sizeof(nextion_config_t));
    
    // Initialize Serial2 for Nextion Display
    NEXTION_SERIAL.begin(gConfig.baudrate, SERIAL_8N1, gConfig.rxPin, gConfig.txPin);
    
    gState = NEXTION_STATE_INITIALIZED;
    LOG_INFO(MODULE, "Initialize success, (RX:%d, TX:%d, %lu baud)", gConfig.rxPin, gConfig.txPin, gConfig.baudrate);
    return SYS_OK;
}

sys_status_t nextion_start(void) {
    if (gState != NEXTION_STATE_INITIALIZED && gState != NEXTION_STATE_STOPPED) {
        LOG_ERROR(MODULE, "Invalid state");
        return SYS_ERR_INVALID_STATE;
    }

    LOG_INFO(MODULE, "Starting...");
    gState = NEXTION_STATE_RUNNING;

    if (xTaskCreatePinnedToCore(nextionTask, "NextionTask", 4096, NULL, 5, &gTaskHandle, 1) != pdPASS) {
        LOG_ERROR(MODULE, "Failed to create task");
        gState = NEXTION_STATE_STOPPED;
        return SYS_ERR_NO_MEMORY;
    }
    return SYS_OK;
}

sys_status_t nextion_stop(void) {
    if (gState != NEXTION_STATE_RUNNING) {
        LOG_ERROR(MODULE, "Invalid state");
        return SYS_ERR_INVALID_STATE;
    }

    LOG_INFO(MODULE, "Stopping...");
    gState = NEXTION_STATE_STOPPED;
    return SYS_OK;
}

nextion_state_t nextion_get_state(void) {
    return gState;
}
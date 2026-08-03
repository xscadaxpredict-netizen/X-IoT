#include "http_client_service.h"
#include "logger.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <HTTPClient.h>
#include <Update.h>
#include <LittleFS.h>

#define MODULE "HTTP_CLIENT"

static http_client_state_t gState = HTTP_CLIENT_STATE_DEINITIALIZED;
static QueueHandle_t gQueue = NULL;
static TaskHandle_t gTaskHandle = NULL;

http_client_state_t http_client_get_state() {
  return gState;
}

// ---------------------------------------------------------
// Internal Download Handlers
// ---------------------------------------------------------
static void handle_ota_update(const char* url) {
  LOG_INFO(MODULE, "Starting OTA from: %s", url);
  
  HTTPClient http; 
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    bool canBegin = Update.begin(contentLength);
    
    if (canBegin) {
      LOG_INFO(MODULE, "Downloading firmware (%d bytes)...", contentLength);
      size_t written = Update.writeStream(http.getStream());
      
      if (written == contentLength && Update.end()) {
        LOG_INFO(MODULE, "OTA Success! Rebooting in 2s...");
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_restart();
      } else {
        LOG_ERROR(MODULE, "OTA Failed: %s", Update.errorString());
      }
    } else {
      LOG_ERROR(MODULE, "Not enough space to begin OTA");
    }
  } else {
    LOG_ERROR(MODULE, "Firmware download failed, HTTP code: %d", httpCode);
  }
  http.end();
}

static void handle_tag_update(const char* url) {
  LOG_INFO(MODULE, "Downloading Tags from: %s", url);
  
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    File file = LittleFS.open("/tag_config.json", "w");
    if (file) {
      http.writeToStream(&file);
      file.close();
      LOG_INFO(MODULE, "Tags Updated! Rebooting in 2s...");
      vTaskDelay(pdMS_TO_TICKS(2000));
      esp_restart();
    } else {
      LOG_ERROR(MODULE, "Failed to open tag_config.json for writing");
    }
  } else {
    LOG_ERROR(MODULE, "Tag download failed, HTTP code: %d", httpCode);
  }
  http.end();
}

// ---------------------------------------------------------
// FreeRTOS Background Task
// ---------------------------------------------------------
static void httpClientTask(void *pv) {
  http_download_request_t req;
  LOG_INFO(MODULE, "Task started");

  while (gState == HTTP_CLIENT_STATE_RUNNING) {
    // Wait infinitely until a request is added to the queue
    if (xQueueReceive(gQueue, &req, portMAX_DELAY) == pdPASS) {
      if (req.action == HTTP_ACTION_OTA_UPDATE) {
        handle_ota_update(req.url);
      } 
      else if (req.action == HTTP_ACTION_TAG_UPDATE) {
        handle_tag_update(req.url);
      }
    }
  }
  gTaskHandle = NULL;
  vTaskDelete(NULL);
}

// ---------------------------------------------------------
// Public APIs
// ---------------------------------------------------------
sys_status_t http_client_download(http_action_t action, const char* url) {
  if (gState != HTTP_CLIENT_STATE_RUNNING) {
    LOG_ERROR(MODULE, "Service is not running");
    return SYS_ERR_INVALID_STATE;
  }
  if (url == NULL) {
    LOG_ERROR(MODULE, "Invalid URL parameter");
    return SYS_ERR_INVALID_PARAM;
  }

  http_download_request_t req;
  req.action = action;
  strncpy(req.url, url, sizeof(req.url) - 1);
  req.url[sizeof(req.url) - 1] = '\0'; // Ensure null termination

  // Add to queue. Will fail if queue is full.
  if (xQueueSend(gQueue, &req, pdMS_TO_TICKS(100)) != pdPASS) {
    LOG_ERROR(MODULE, "Download queue is full");
    return SYS_ERR_FAIL;
  }
  
  LOG_INFO(MODULE, "Download requested successfully");
  return SYS_OK;
}

sys_status_t http_client_init() {
  if (gState != HTTP_CLIENT_STATE_DEINITIALIZED) return SYS_ERR_INVALID_STATE;
  
  LOG_INFO(MODULE, "Initializing...");
  
  // Create a queue that holds up to 5 download requests
  gQueue = xQueueCreate(5, sizeof(http_download_request_t));
  if (gQueue == NULL) {
    LOG_ERROR(MODULE, "Failed to create queue");
    return SYS_ERR_NO_MEMORY;
  }
  
  gState = HTTP_CLIENT_STATE_INITIALIZED;
  LOG_INFO(MODULE, "Initialize success!");
  return SYS_OK;
}

sys_status_t http_client_start() {
  if (gState != HTTP_CLIENT_STATE_INITIALIZED && gState != HTTP_CLIENT_STATE_STOPPED) {
    return SYS_ERR_INVALID_STATE;
  }
  
  LOG_INFO(MODULE, "Starting...");

  gState = HTTP_CLIENT_STATE_RUNNING;
  if (xTaskCreatePinnedToCore(httpClientTask, "http_client", 8192, NULL, 5, &gTaskHandle, 1) != pdPASS) {
    gState = HTTP_CLIENT_STATE_INITIALIZED;
    LOG_ERROR(MODULE, "Failed to create task");
    return SYS_ERR_NO_MEMORY;
  }

  return SYS_OK;
}

sys_status_t http_client_stop() {
  if (gState != HTTP_CLIENT_STATE_RUNNING) return SYS_ERR_INVALID_STATE;
  
  LOG_INFO(MODULE, "Stopping...");  
  gState = HTTP_CLIENT_STATE_STOPPED;

  return SYS_OK;
}
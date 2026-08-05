#include "web_server.h"
#include <PsychicHttp.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "logger.h"
#include "wifi_service.h"
#include "mqtt_service.h"
#include "esp_timer.h"
#include "tag_registry.h"
#include "publisher.h"
#include "system_service.h"

#define MODULE "WEB_SERVER"
#define JSON_BUF_SIZE 2056

static PsychicHttpServer server;
static File uploadFile;
static bool gServerRunning = false;
static portal_config_t gConfig;

// Declare the Authentication Middleware object globally
static AuthenticationMiddleware basicAuth;

sys_status_t web_server_init(const portal_config_t* cfg) {
  LOG_INFO(MODULE, "Initializing...");
  memcpy(&gConfig, cfg, sizeof(portal_config_t));

  // Configure the Basic Auth Middleware once at startup
  basicAuth.setUsername(gConfig.username);
  basicAuth.setPassword(gConfig.password);
  basicAuth.setRealm("Secure Setup Portal");
  basicAuth.setAuthMethod(BASIC_AUTH);

  // 1. Route Root / (Serves index.html)
  server.on("/", HTTP_GET, [](PsychicRequest *request, PsychicResponse *response) {
    if (LittleFS.exists("/index.html")) {
      PsychicFileResponse fileResponse(response, LittleFS, "/index.html", "text/html");
      return fileResponse.send();
    }
    response->setCode(404);
    response->setContentType("text/plain");
    response->setContent("index.html missing");
    return response->send();
  })->addMiddleware(&basicAuth);

  // 2. GET API: Send system configuration JSON
  server.on("/api/config", HTTP_GET, [](PsychicRequest *request, PsychicResponse *response) {
    if (LittleFS.exists("/system_config.json")) {
      PsychicFileResponse fileResponse(response, LittleFS, "/system_config.json", "application/json");
      return fileResponse.send();
    }
    response->setCode(404);
    response->setContentType("text/plain");
    response->setContent("system_config.json missing");
    return response->send();
  })->addMiddleware(&basicAuth);

  // 3. POST API: Receive and save system configuration JSON
  server.on("/api/config", HTTP_POST, [](PsychicRequest *request, PsychicResponse *response) {
    if (request->request()->content_len >= JSON_BUF_SIZE) {
      response->setCode(400);
      response->setContentType("application/json");
      response->setContent("{\"status\":\"error\",\"message\":\"Payload too large\"}");
      return response->send();
    }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, request->body().c_str());
    if (err) {
      response->setCode(400);
      response->setContentType("application/json");
      response->setContent("{\"status\":\"error\",\"message\":\"Invalid JSON content\"}");
      return response->send();
    }

    File file = LittleFS.open("/system_config.json", "w");
    if (!file) {
      response->setCode(500);
      response->setContentType("application/json");
      response->setContent("{\"status\":\"error\",\"message\":\"Failed to save settings\"}");
      return response->send();
    }
    serializeJson(doc, file);
    file.close();

    response->setCode(200);
    response->setContentType("application/json");
    response->setContent("{\"status\":\"ok\",\"message\":\"Saved. Rebooting...\"}");
    esp_err_t ret = response->send();

    LOG_INFO(MODULE, "Settings saved. Rebooting...");
    vTaskDelay(pdMS_TO_TICKS(2000));
    esp_restart();
    return ret;
  })->addMiddleware(&basicAuth);

  // 4. POST API: Upload tag_config.json (using PsychicUploadHandler)
  PsychicUploadHandler *tagUploadHandler = new PsychicUploadHandler();

  tagUploadHandler->onUpload([](PsychicRequest *request, const String& filename, uint64_t index, uint8_t *data, size_t len, bool final) {
    if (index == 0) {
      LOG_INFO(MODULE, "Tag upload starting: %s", filename.c_str());
      uploadFile = LittleFS.open("/tag_config.json", "w");
      if (!uploadFile) {
        LOG_ERROR(MODULE, "Failed to open /tag_config.json for writing");
        return ESP_FAIL;
      }
    }

    if (uploadFile) {
      uploadFile.write(data, len);
    }

    if (final) {
      if (uploadFile) {
        uploadFile.close();
        LOG_INFO(MODULE, "Tag upload complete");
      }
    }
    return ESP_OK;
  });

  tagUploadHandler->onRequest([](PsychicRequest *request, PsychicResponse *response) {
    response->setCode(200);
    response->setContentType("application/json");
    response->setContent("{\"status\":\"ok\",\"message\":\"Tags saved\"}");
    return response->send();
  });

  server.on("/api/upload-tags", HTTP_POST, tagUploadHandler)->addMiddleware(&basicAuth);

  // 5. POST API: Reboot
  server.on("/api/reboot", HTTP_POST, [](PsychicRequest *request, PsychicResponse *response) {
    response->setCode(200);
    response->setContentType("application/json");
    response->setContent("{\"status\":\"ok\",\"message\":\"Rebooting...\"}");
    esp_err_t ret = response->send();

    vTaskDelay(pdMS_TO_TICKS(2000));
    esp_restart();
    return ret;
  })->addMiddleware(&basicAuth);

  // 6. GET API: System Health Dashboard
  server.on("/api/system", HTTP_GET, [](PsychicRequest *request, PsychicResponse *response) {
    JsonDocument doc;
    // ── System Overview ──
    doc["uptime"] = esp_timer_get_time() / 1000000;
    doc["freeHeap"] = ESP.getFreeHeap();
    doc["minFreeHeap"] = ESP.getMinFreeHeap();
    doc["cpuTemp"] = temperatureRead();
    doc["fwVersion"] = GATEWAY_VERSION;
    // ── Flash Storage ──
    doc["flashTotal"] = LittleFS.totalBytes();
    doc["flashUsed"] = LittleFS.usedBytes();
    // ── Wi-Fi ──
    connection_state_t wifiState = wifi_get_connection_state();
    doc["wifiStatus"] = wifi_is_connected();
    doc["wifiStateCode"] = (int)wifiState;
    if (wifiState == WIFI_CONNECTION_STATE_GOT_IP) {
      doc["ipAddress"] = WiFi.localIP().toString();
      doc["rssi"] = WiFi.RSSI();
    } else if (wifiState == WIFI_CONNECTION_STATE_CONNECTED) {
      doc["ipAddress"] = "Acquiring IP...";
      doc["rssi"] = WiFi.RSSI();
    } else {
      doc["ipAddress"] = "0.0.0.0";
    }
    // ── MQTT ──
    doc["mqttStatus"] = (mqtt_get_connection_state() == MQTT_CONN_CONNECTED);
    // ── Fieldbus ──
    doc["activeTags"] = tag_count();
    doc["publisherStatus"] = (publisher_get_state() == PUBLISHER_STATE_RUNNING);
    String jsonResponse;
    serializeJson(doc, jsonResponse);
    response->setCode(200);
    response->setContentType("application/json");
    response->setContent(jsonResponse.c_str());
    return response->send();
  })->addMiddleware(&basicAuth);

  // 7. Captive Portal Redirect rule
  server.onNotFound([](PsychicRequest *request, PsychicResponse *response) {
    response->setCode(302);
    response->addHeader("Location", "http://192.168.4.1/");
    return response->send();
  });

  return SYS_OK;
}

sys_status_t web_server_start(void) {
  if (gServerRunning) return SYS_OK;

  if(server.begin() != ESP_OK){
    LOG_ERROR(MODULE, "Failed to start the server");
    return SYS_ERR_FAIL;
  }

  gServerRunning = true;
  LOG_INFO(MODULE, "Sever running on Port 80");
  return SYS_OK;
}

sys_status_t web_server_stop(void) {
  if (!gServerRunning) return SYS_OK;

  LOG_INFO(MODULE, "Stopping Web Server...");

  if(server.end() != ESP_OK){
    LOG_ERROR(MODULE, "Failed to stop the server");
    return SYS_ERR_FAIL;
  }

  gServerRunning = false;
  LOG_INFO(MODULE, "Stopped the server");
  return SYS_OK;
}
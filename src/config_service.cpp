#include "config_service.h"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "tag_registry.h"
#include "logger.h"

#define MODULE "CONFIG"

static app_wifi_config_t gWifiConfig;
static mqtt_config_t gMqttConfig;
static modbus_config_t gModbusConfig;
static acquisition_config_t gAcqConfig;
static publisher_config_t gPubConfig;
static nextion_config_t gNxtConfig;

static bool gConfigLoaded = false;

static uint32_t parse_serial_config(const char* cfgStr) {
  if (cfgStr == NULL) return SERIAL_8N1;
  if (strcmp(cfgStr, "SERIAL_8N1") == 0) return SERIAL_8N1;
  if (strcmp(cfgStr, "SERIAL_8E1") == 0) return SERIAL_8E1;
  if (strcmp(cfgStr, "SERIAL_8O1") == 0) return SERIAL_8O1;
  return SERIAL_8N1;
}

static word_order_t parse_word_order(const char* orderStr) {
  if (orderStr == NULL) return WORD_CDAB;
  if (strcmp(orderStr, "WORD_ABCD") == 0) return WORD_ABCD;
  if (strcmp(orderStr, "WORD_CDAB") == 0) return WORD_CDAB;
  if (strcmp(orderStr, "WORD_BADC") == 0) return WORD_BADC;
  if (strcmp(orderStr, "WORD_DCBA") == 0) return WORD_DCBA;
  return WORD_CDAB;
}

static tag_topic_type_t parse_topic_type(const char* typeStr) {
  if (typeStr == NULL) return TAG_TOPIC_TYPE_PUBLISH;
  if (strcmp(typeStr, "SUBSCRIBE") == 0) return TAG_TOPIC_TYPE_SUBSCRIBE;
  if (strcmp(typeStr, "PUBLISH") == 0) return TAG_TOPIC_TYPE_PUBLISH;
  return TAG_TOPIC_TYPE_PUBLISH;
}

static tag_source_t parse_source(const char* srcStr) {
  if (srcStr == NULL) return TAG_SOURCE_MODBUS;
  if (strcmp(srcStr, "MODBUS") == 0) return TAG_SOURCE_MODBUS;
  if (strcmp(srcStr, "CAN") == 0) return TAG_SOURCE_CAN;
  if (strcmp(srcStr, "SYSTEM") == 0) return TAG_SOURCE_SYSTEM;
  return TAG_SOURCE_MODBUS;
}

static tag_access_t parse_access(const char* accStr) {
  if (accStr == NULL) return TAG_ACCESS_READ_ONLY;
  if (strcmp(accStr, "READ_ONLY") == 0) return TAG_ACCESS_READ_ONLY;
  if (strcmp(accStr, "WRITE_ONLY") == 0) return TAG_ACCESS_WRITE_ONLY;
  if (strcmp(accStr, "READ_WRITE") == 0) return TAG_ACCESS_READ_WRITE;
  return TAG_ACCESS_READ_ONLY;
}

static tag_type_t parse_tag_type(const char* typeStr) {
  if (typeStr == NULL) return (tag_type_t)0;
  if (strcmp(typeStr, "TAG_BOOL") == 0) return TAG_BOOL;
  if (strcmp(typeStr, "TAG_UINT16") == 0) return TAG_UINT16;
  if (strcmp(typeStr, "TAG_INT16") == 0) return TAG_INT16;
  if (strcmp(typeStr, "TAG_UINT32") == 0) return TAG_UINT32;
  if (strcmp(typeStr, "TAG_INT32") == 0) return TAG_INT32;
  if (strcmp(typeStr, "TAG_FLOAT32") == 0) return TAG_FLOAT32;
  if (strcmp(typeStr, "TAG_STRING") == 0) return TAG_STRING;
  return (tag_type_t)0;
}

static sys_status_t load_system_config() {
  File file = LittleFS.open("/system_config.json", "r");
  if (!file) {
    LOG_ERROR(MODULE, "Failed to open /system_config.json");
    return SYS_ERR_FAIL;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    LOG_ERROR(MODULE, "Failed to parse /system_config.json: %s", error.c_str());
    return SYS_ERR_FAIL;
  }

  // Parse Wifi Config
  JsonObject wifi = doc["wifi"];
  gWifiConfig.ssid = strdup(wifi["ssid"] | "abcd");
  gWifiConfig.password = strdup(wifi["password"] | "1234");
  gWifiConfig.autoReconnect = wifi["autoReconnect"] | true;
  gWifiConfig.reconnectTimeoutMs = wifi["reconnectTimeoutMs"] | 20000;
  gWifiConfig.mode = WIFI_STA;

  // Parse MQTT Config
  JsonObject mqtt = doc["mqtt"];
  gMqttConfig.broker = strdup(mqtt["broker"] | "www.abcd.com");
  gMqttConfig.port = mqtt["port"] | 1883;
  gMqttConfig.username = strdup(mqtt["username"] | "abcd");
  gMqttConfig.password = strdup(mqtt["password"] | "1234");
  gMqttConfig.clientId = strdup(mqtt["clientId"] | "abcd");
  gMqttConfig.secure = mqtt["secure"] | false;
  gMqttConfig.cleanSession = mqtt["cleanSession"] | false;
  gMqttConfig.autoReconnect = mqtt["autoReconnect"] | true;
  gMqttConfig.keepAlive = mqtt["keepAlive"] | 120;

  // Parse Modbus Config
  JsonObject mb = doc["modbus"];
  gModbusConfig.baudrate = mb["baudrate"] | 9600;
  gModbusConfig.serialConfig = parse_serial_config(mb["serialConfig"] | "SERIAL_8N1");
  gModbusConfig.txPin = mb["txPin"] | 17;
  gModbusConfig.rxPin = mb["rxPin"] | 18;
  gModbusConfig.timeoutMs = mb["timeoutMs"] | 2000;

  // Parse Acquisition Config
  gAcqConfig.scanIntervalMs = doc["acquisition"]["scanIntervalMs"] | 10000;

  // Parse Publisher Config
  gPubConfig.publishIntervalMs = doc["publisher"]["publishIntervalMs"] | 20000;

  // Parse Nextion (Using Exposed GPIO 1 & 2 for Waveshare board)
  JsonObject nxt = doc["nextion"];
  gNxtConfig.rxPin = nxt["rxPin"] | 1;
  gNxtConfig.txPin = nxt["txPin"] | 2;
  gNxtConfig.baudrate = nxt["baudrate"] | 9600;
  gNxtConfig.intervalMs = nxt["intervalMs"] | 2000;

  LOG_INFO(MODULE, "System configurations loaded successfully");
  return SYS_OK;
}

static sys_status_t load_tags_config() {
  File file = LittleFS.open("/tag_config.json", "r");
  if (!file) {
    LOG_ERROR(MODULE, "Failed to open /tag_config.json");
    return SYS_ERR_FAIL;
  }

  JsonDocument doc; 
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    LOG_ERROR(MODULE, "Failed to parse /tag_config.json: %s", error.c_str());
    return SYS_ERR_FAIL;
  }

  JsonArray tagList = doc.as<JsonArray>();
  uint16_t count = tagList.size();
  
  sys_status_t status = tag_registry_init(count);
  if (status != SYS_OK) return status;

  for (JsonObject jsonTag : tagList) {
    tag_config_t tag;
    memset(&tag, 0, sizeof(tag_config_t));

    tag.tagId = jsonTag["tagId"];
    tag.name = strdup(jsonTag["name"] | "");
    tag.unit = strdup(jsonTag["unit"] | "");
    tag.topic = strdup(jsonTag["topic"] | "");
    tag.topicType = parse_topic_type(jsonTag["topicType"]);
    tag.source = parse_source(jsonTag["source"]);
    tag.access = parse_access(jsonTag["access"]);
    tag.dataType = parse_tag_type(jsonTag["dataType"]);
    tag.valueDataType = parse_tag_type(jsonTag["valueDataType"]); 
    tag.multiplier = jsonTag["multiplier"] | 1.0f;
    tag.offset = jsonTag["offset"] | 0.0f;
    tag.nxtComponent = jsonTag["nxtComponent"].isNull() ? NULL : strdup(jsonTag["nxtComponent"]);

    // Parse Bool Map
    if (jsonTag.containsKey("boolMap") && !jsonTag["boolMap"].isNull()) {
      JsonObject bm = jsonTag["boolMap"];
      bool_map_t* bMap = (bool_map_t*)malloc(sizeof(bool_map_t));
      bMap->falseVal = strdup(bm["falseVal"] | "OFF");
      bMap->trueVal = strdup(bm["trueVal"] | "ON");
      tag.boolMap = bMap;
    }

    // Parse Enum Map
    if (jsonTag.containsKey("enumMap") && !jsonTag["enumMap"].isNull()) {
      JsonArray em = jsonTag["enumMap"];
      tag.enumMapSize = em.size();
      enum_map_t* eMap = (enum_map_t*)malloc(sizeof(enum_map_t) * tag.enumMapSize);
      for (uint16_t i = 0; i < tag.enumMapSize; i++) {
        eMap[i].value = em[i]["val"];
        eMap[i].name = strdup(em[i]["name"] | "");
      }
      tag.enumMap = eMap;
    }

    // Parse Modbus Config
    if (jsonTag.containsKey("modbus")) {
      JsonObject mb = jsonTag["modbus"];
      tag.sourceConfig.modbus.slaveId = mb["slaveId"] | 1;
      tag.sourceConfig.modbus.address = mb["address"] | 0;
      tag.sourceConfig.modbus.quantity = mb["quantity"] | 1;
      tag.sourceConfig.modbus.functionCode = mb["functionCode"] | 3;
      tag.sourceConfig.modbus.wordOrder = parse_word_order(mb["wordOrder"] | "WORD_CDAB");
    }

    status = tag_registry_add(&tag);
    if (status != SYS_OK) {
      LOG_ERROR(MODULE, "Failed to insert tag ID %lu", tag.tagId);
    }
  }

  LOG_INFO(MODULE, "Loaded %d tags into registry", tag_count());
  return SYS_OK;
}

sys_status_t config_init(void) {
  if (gConfigLoaded) return SYS_OK;

  LOG_INFO(MODULE, "Mounting LittleFS filesystem...");
  if (!LittleFS.begin(true)) {
    LOG_ERROR(MODULE, "LittleFS Mount Failed!");
    return SYS_ERR_FAIL;
  }

  sys_status_t status;
  status = load_system_config();
  if (status != SYS_OK) return status;

  status = load_tags_config();
  if (status != SYS_OK) return status;

  gConfigLoaded = true;
  return SYS_OK;
}

const app_wifi_config_t* config_get_wifi(void) { return &gWifiConfig; }
const mqtt_config_t* config_get_mqtt(void) { return &gMqttConfig; }
const modbus_config_t* config_get_modbus(void) { return &gModbusConfig; }
const acquisition_config_t* config_get_acq(void) { return &gAcqConfig; }
const publisher_config_t* config_get_publisher(void) { return &gPubConfig; }
const nextion_config_t* config_get_nextion(void) { return &gNxtConfig; }
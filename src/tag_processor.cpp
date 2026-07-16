#include "tag_processor.h"
#include "logger.h"
#include <string.h>

#define MODULE "TAG_PROCESSOR"

sys_status_t tag_process(tag_runtime_t* tag) {
  if (tag == NULL || tag->config == NULL) {
    return SYS_ERR_INVALID_PARAM;
  }

  const tag_config_t* cfg = tag->config;

  // Determine actual output datatype (defaults to dataType if valueDataType is 0/unspecified)
  tag_type_t valType = (cfg->valueDataType == 0) ? cfg->dataType : cfg->valueDataType;

  // 1. Handle Mapping to String (Enum mapping or custom Boolean string mapping)
  if (valType == TAG_STRING) {
    if (cfg->dataType == TAG_BOOL) {
      bool val = tag->rawValue.bv;
      if (cfg->boolMap != NULL) {
        const char* mappedStr = val ? cfg->boolMap->trueVal : cfg->boolMap->falseVal;
        strncpy(tag->value.strv, mappedStr, sizeof(tag->value.strv) - 1);
      } 
      else {
        strncpy(tag->value.strv, val ? "ON" : "OFF", sizeof(tag->value.strv) - 1);
      }
      tag->value.strv[sizeof(tag->value.strv) - 1] = '\0';
      return SYS_OK;
    } 
    else {
      // Integer to Enum descriptive string mapping
      int intVal = 0;
      switch(cfg->dataType) {
        case TAG_UINT16: intVal = tag->rawValue.u16v; break;
        case TAG_INT16:  intVal = tag->rawValue.i16v; break;
        case TAG_UINT32: intVal = tag->rawValue.u32v; break;
        case TAG_INT32:  intVal = tag->rawValue.i32v; break;
        default:
          LOG_ERROR(MODULE, "Invalid raw dataType for enum string mapping: %d", cfg->dataType);
          return SYS_ERR_FAIL;
      }
      
      bool found = false;
      if (cfg->enumMap != NULL) {
        for (uint16_t i = 0; i < cfg->enumMapSize; i++) {
          if (cfg->enumMap[i].value == intVal) {
            strncpy(tag->value.strv, cfg->enumMap[i].name, sizeof(tag->value.strv) - 1);
            found = true;
            break;
          }
        }
      }
      if (!found) {
        // Fallback to raw value representation if enum mapping is missing/unmatched
        snprintf(tag->value.strv, sizeof(tag->value.strv), "%d", intVal);
      }
      tag->value.strv[sizeof(tag->value.strv) - 1] = '\0';
      return SYS_OK;
    }
  }

  // 2. Handle Simple Boolean output
  if (valType == TAG_BOOL) {
    tag->value.bv = (cfg->dataType == TAG_BOOL) ? tag->rawValue.bv : (tag->rawValue.u16v != 0);
    return SYS_OK;
  }

  // 3. Float-based scaling calculations
  float multiplier = (cfg->multiplier == 0.0f) ? 1.0f : cfg->multiplier;
  float rawVal = 0.0f;
  switch (cfg->dataType) {
    case TAG_UINT16:  rawVal = (float)tag->rawValue.u16v; break;
    case TAG_INT16:   rawVal = (float)tag->rawValue.i16v; break;
    case TAG_UINT32:  rawVal = (float)tag->rawValue.u32v; break;
    case TAG_INT32:   rawVal = (float)tag->rawValue.i32v; break;
    case TAG_FLOAT32: rawVal = tag->rawValue.f32v; break;
    case TAG_BOOL:    rawVal = tag->rawValue.bv ? 1.0f : 0.0f; break;
    default:
      LOG_ERROR(MODULE, "Unsupported raw dataType: %d", cfg->dataType);
      return SYS_ERR_FAIL;
  }

  float scaledValue = (rawVal * multiplier) + cfg->offset;

  // 4. Store scaled value into engineered value field
  switch (valType) {
    case TAG_UINT16:  tag->value.u16v = (uint16_t)scaledValue; break;
    case TAG_INT16:   tag->value.i16v = (int16_t)scaledValue; break;
    case TAG_UINT32:  tag->value.u32v = (uint32_t)scaledValue; break;
    case TAG_INT32:   tag->value.i32v = (int32_t)scaledValue; break;
    case TAG_FLOAT32: tag->value.f32v = scaledValue; break;
    default:
      LOG_ERROR(MODULE, "Unsupported processed value dataType: %d", valType);
      return SYS_ERR_FAIL;
  }

  return SYS_OK;
}
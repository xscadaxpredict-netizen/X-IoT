#include "tag_registry.h"
#include <string.h>
#include <stdlib.h>
#include "logger.h"

#define MODULE "TAG_REGISTRY"

static tag_config_t* gTags = NULL;
static uint16_t gCapacity = 0;
static uint16_t gTagCount = 0;

sys_status_t tag_registry_init(uint16_t capacity) {
  if (gTags != NULL) {
    for (uint16_t i = 0; i < gTagCount; i++) {
      if (gTags[i].name) free((void*)gTags[i].name);
      if (gTags[i].unit) free((void*)gTags[i].unit);
      if (gTags[i].topic) free((void*)gTags[i].topic);
      if (gTags[i].nxtComponent) free((void*)gTags[i].nxtComponent);
      if (gTags[i].boolMap) {
        if (gTags[i].boolMap->falseVal) free((void*)gTags[i].boolMap->falseVal);
        if (gTags[i].boolMap->trueVal) free((void*)gTags[i].boolMap->trueVal);
        free((void*)gTags[i].boolMap);
      }
      if (gTags[i].enumMap) {
        for (uint16_t j = 0; j < gTags[i].enumMapSize; j++) {
          if (gTags[i].enumMap[j].name) free((void*)gTags[i].enumMap[j].name);
        }
        free((void*)gTags[i].enumMap);
      }
    }
    free(gTags);
    gTags = NULL;
  }

  gCapacity = capacity;
  gTagCount = 0;
  gTags = (tag_config_t*)calloc(gCapacity, sizeof(tag_config_t));
  if (gTags == NULL) {
    LOG_ERROR(MODULE, "Failed to allocate memory for tag registry");
    return SYS_ERR_NO_MEMORY;
  }
  LOG_INFO(MODULE, "Registry initialized with capacity: %u", gCapacity);
  return SYS_OK;
}

sys_status_t tag_registry_add(const tag_config_t* tag) {
  if (gTags == NULL) {
    return SYS_ERR_INVALID_STATE;
  }
  if (tag == NULL) {
    return SYS_ERR_INVALID_PARAM;
  }
  if (gTagCount >= gCapacity) {
    LOG_ERROR(MODULE, "Registry is full! Capacity: %u", gCapacity);
    return SYS_ERR_FAIL;
  }
  // Copy standard values
  gTags[gTagCount] = *tag;
  gTagCount++;
  return SYS_OK;
}

uint16_t tag_count(){
  return gTagCount;
}

const tag_config_t* tag_find_by_id(uint16_t tagId){
  if (gTags == NULL) return NULL;
  for(uint16_t i = 0; i < gTagCount; i++){
    if(gTags[i].tagId == tagId){
      return &gTags[i];
    }
  }
  return NULL;
}

const tag_config_t* tag_find_by_name(const char* name){
  if(gTags == NULL || name == NULL) return NULL;
  for(uint16_t i = 0; i < gTagCount; i++){
    if(gTags[i].name != NULL && strcmp(gTags[i].name, name) == 0){
      return &gTags[i];
    }
  }
  return NULL;
}

const tag_config_t* tag_find_by_topic(const char* topic){
  if(gTags == NULL || topic == NULL) return NULL;
  
  for(uint16_t i = 0; i < gTagCount; i++){
    if(gTags[i].topic != NULL && strcmp(gTags[i].topic, topic) == 0){
      return &gTags[i];
    }
  }
  return NULL;
}

const tag_config_t* tag_get_at(uint16_t index){
  if(gTags == NULL || index >= gTagCount){
    return NULL;
  }
  return &gTags[index];
}
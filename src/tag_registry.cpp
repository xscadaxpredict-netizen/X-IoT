#include "tag_registry.h"
#include <string.h>

static tag_config_t gTags[] = {
  // relay status and control start
  {
  .tagId = 1,
  .name = "MACH-YC",
  .unit = "Status",
  .topic = "HDR0029/MACH-YC-100",  // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 0, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 2,
  .name = "MODE-YC",
  .unit = "Status",
  .topic = "HDR0029/MODE-YC-100",  // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 1, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 3,
  .name = "RSP-YC",
  .unit = "Status",
  .topic = "HDR0029/RSP-YC-101",  // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 2, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 4,
  .name = "AIB-YC",
  .unit = "Status",
  .topic = "HDR0029/AIB_01-YC-102",  // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 3, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 5,
  .name = "FFP-YC",
  .unit = "Status",
  .topic = "HDR0029/FFP-YC-103",  // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 4, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 6,
  .name = "PSS-YC",
  .unit = "Status",
  .topic = "HDR0029/PSS-YC-104",  // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 5, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 7,
  .name = "MACH-YT",
  .unit = "Status",
  .topic = "HDR0029/MACH-YT-100",  // Y-Status, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 0, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 8,
  .name = "MODE-YT",
  .unit = "Status",
  .topic = "HDR0029/MODE-YT-100",  // Y-Status, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 1, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 9,
  .name = "RSP-YT",
  .unit = "Status",
  .topic = "HDR0029/RSP-YT-101",  // Y-Status, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 2, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 10,
  .name = "AIB-YT",
  .unit = "Status",
  .topic = "HDR0029/AIB_01-YT-102",  // Y-Status, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 3, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 11,
  .name = "FFP-YT",
  .unit = "Status",
  .topic = "HDR0029/FFP-YT-103",  // Y-Status, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 4, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 12,
  .name = "PSS-YT",
  .unit = "Status",
  .topic = "HDR0029/PSS-YT-104",  // Y-Status, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 1, 
        .address = 5, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  // relay status and control end
  // dummy tag start
  {
  .tagId = 13,
  .name = "DUMMY-01",
  .unit = "Status",
  .topic = "HDR0029/DUMMY-01",
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 0, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_ABCD 
      }
    }
  },
  // dummy tag end
  // current sensors start
  {
  .tagId = 14,
  .name = "MACH-IT",
  .unit = "Status",
  .topic = "HDR0029/MACH-IT-100",  // I-Current, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 0, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_ABCD 
      }
    }
  },
  {
  .tagId = 15,
  .name = "RSP-IT",
  .unit = "Status",
  .topic = "HDR0029/RSP-IT-101",  // I-Current, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 1, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_ABCD 
      }
    }
  },
  {
  .tagId = 16,
  .name = "AIB_01-IT",
  .unit = "Status",
  .topic = "HDR0029/AIB_01-IT-102",  // I-Current, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 2, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_ABCD 
      }
    }
  },
  {
  .tagId = 17,
  .name = "FFP-IT",
  .unit = "Status",
  .topic = "HDR0029/FFP-IT-103",  // I-Current, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 3, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_ABCD 
      }
    }
  },
  {
  .tagId = 18,
  .name = "PSS-IT",
  .unit = "Status",
  .topic = "HDR0029/PSS-IT-104",  // I-Current, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 4, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_ABCD 
      }
    }
  },
  {
  .tagId = 19,
  .name = "AIB_2-IT",
  .unit = "Status",
  .topic = "HDR0029/AIB_2-IT-105",  // I-Current, T-Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 5, 
        .quantity = 1, 
        .functionCode = 0x04,
        .wordOrder = WORD_ABCD 
      }
    }
  },
};

static const uint16_t gTagCount = sizeof(gTags) / sizeof(gTags[0]);

uint16_t tag_count(){
  return gTagCount;
}

const tag_config_t* tag_find_by_id(uint16_t tagId){
  for(uint16_t i = 0; i < gTagCount; i++){
    if(gTags[i].tagId == tagId){
      return &gTags[i];
    }
  }
  return NULL;
}

const tag_config_t* tag_find_by_name(const char* name){
  if(name == NULL) return NULL;
  
  for(uint16_t i = 0; i < gTagCount; i++){
    if(gTags[i].name != NULL && strcmp(gTags[i].name, name) == 0){
      return &gTags[i];
    }
  }
  return NULL;
}

const tag_config_t* tag_find_by_topic(const char* topic){
  if(topic == NULL) return NULL;
  
  for(uint16_t i = 0; i < gTagCount; i++){
    if(gTags[i].topic != NULL && strcmp(gTags[i].topic, topic) == 0){
      return &gTags[i];
    }
  }
  return NULL;
}

const tag_config_t* tag_get_at(uint16_t index){
  if(index >= gTagCount){
    return NULL;
  }
  return &gTags[index];
}
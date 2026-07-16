#include "tag_registry.h"
#include <string.h>

static const bool_map_t gOnOffMap = { .falseVal = "OFF", .trueVal = "ON" };
static const bool_map_t gAutoManualMap = { .falseVal = "AUTO", .trueVal = "MANUAL" };
static const bool_map_t gHighLowMap = { .falseVal = "LOW", .trueVal = "HIGH" };
static const enum_map_t gProcessStateMap[] = {
  { 100, "IDLE" },
  { 101, "FILL_TANK" },
  { 102, "EC_PROCESS" },
  { 103, "STIRRER_PROCESS" },
  { 104, "STAGNATION" },
  { 105, "VALVE_OPEN" },
  { 106, "WAIT_DRAIN" },
  { 107, "VALVE_CLOSE" },
  { 108, "DSP_PROCESS" },
  { 109, "NEXT_PROCESS" },

};

static tag_config_t gTags[] = {
  {
  .tagId = 1,
  .name = "MODE-YC",
  .unit = "Status",
  .topic = "XP/TW-01/MODE-YC-100",  // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 12, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 2,
  .name = "MODE-YT",
  .unit = "Status",
  .topic = "XP/TW-01/MODE-YT-100",  // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gAutoManualMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 12, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 3,
  .name = "RSP-YC",
  .unit = "Status",
  .topic = "XP/TW-01/RSP-YC-101", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 13, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 4,
  .name = "RSP-YT",
  .unit = "Status",
  .topic = "XP/TW-01/RSP-YT-101", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gOnOffMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 13, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 5,
  .name = "EC-YC",
  .unit = "Status",
  .topic = "XP/TW-01/EC-YC-102", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 14, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  // {
  // .tagId = 6,
  // .name = "EC-YT",
  // .unit = "Status",
  // .topic = "XP/TW-01/EC-YT-102", // Y-Status, T-Feedback/Transmit
  // .topicType = TAG_TOPIC_TYPE_PUBLISH,
  // .source = TAG_SOURCE_MODBUS,
  // .access = TAG_ACCESS_READ_ONLY,
  // .dataType = TAG_BOOL,
  // .valueDataType = TAG_STRING,
  // .boolMap = &gOnOffMap,
  // .sourceConfig = {
  //     .modbus = {
  //       .slaveId = 2, 
  //       .address = 14, 
  //       .quantity = 1, 
  //       .functionCode = 0x01, 
  //       .wordOrder = WORD_CDAB 
  //     }
  //   }
  // },
  {
  .tagId = 7,
  .name = "EC_PLUS-YC",
  .unit = "Status",
  .topic = "XP/TW-01/EC_PLUS-YC-102", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 15, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  // {
  // .tagId = 8,
  // .name = "EC_PLUS-YT",
  // .unit = "Status",
  // .topic = "XP/TW-01/EC_PLUS-YT-102", // Y-Status, T-Feedback/Transmit
  // .topicType = TAG_TOPIC_TYPE_PUBLISH,
  // .source = TAG_SOURCE_MODBUS,
  // .access = TAG_ACCESS_READ_ONLY,
  // .dataType = TAG_BOOL,
  // .sourceConfig = {
  //     .modbus = {
  //       .slaveId = 2, 
  //       .address = 15, 
  //       .quantity = 1, 
  //       .functionCode = 0x01, 
  //       .wordOrder = WORD_CDAB 
  //     }
  //   }
  // },
  {
  .tagId = 9,
  .name = "EC_MINUS-YC",
  .unit = "Status",
  .topic = "XP/TW-01/EC_MINUS-YC-102", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 16, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  // {
  // .tagId = 10,
  // .name = "EC_MINUS-YT",
  // .unit = "Status",
  // .topic = "XP/TW-01/EC_MINUS-YT-102", // Y-Status, T-Feedback/Transmit
  // .topicType = TAG_TOPIC_TYPE_PUBLISH,
  // .source = TAG_SOURCE_MODBUS,
  // .access = TAG_ACCESS_READ_ONLY,
  // .dataType = TAG_BOOL,
  // .sourceConfig = {
  //     .modbus = {
  //       .slaveId = 2, 
  //       .address = 16, 
  //       .quantity = 1, 
  //       .functionCode = 0x01, 
  //       .wordOrder = WORD_CDAB 
  //     }
  //   }
  // },
  {
  .tagId = 11,
  .name = "STR-YC",
  .unit = "Status",
  .topic = "XP/TW-01/STR-YC-103", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 17, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  // {
  // .tagId = 12,
  // .name = "STR-YT",
  // .unit = "Status",
  // .topic = "XP/TW-01/STR-YT-103", // Y-Status, T-Feedback/Transmit
  // .topicType = TAG_TOPIC_TYPE_PUBLISH,
  // .source = TAG_SOURCE_MODBUS,
  // .access = TAG_ACCESS_READ_ONLY,
  // .dataType = TAG_BOOL,
  // .sourceConfig = {
  //     .modbus = {
  //       .slaveId = 2, 
  //       .address = 17, 
  //       .quantity = 1, 
  //       .functionCode = 0x01, 
  //       .wordOrder = WORD_CDAB 
  //     }
  //   }
  // },
  {
  .tagId = 13,
  .name = "EVO-YC",
  .unit = "Status",
  .topic = "XP/TW-01/EVO-YC-104", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 18, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 14,
  .name = "EVO-YT",
  .unit = "Status",
  .topic = "XP/TW-01/EVO-YT-104", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gOnOffMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 18, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 15,
  .name = "EVC-YC",
  .unit = "Status",
  .topic = "XP/TW-01/EVC-YC-104", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 19, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 16,
  .name = "EVC-YT",
  .unit = "Status",
  .topic = "XP/TW-01/EVC-YT-104", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gOnOffMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 19, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 17,
  .name = "DSP-YC",
  .unit = "Status",
  .topic = "XP/TW-01/DSP-YC-105", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 20, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  // {
  // .tagId = 18,
  // .name = "DSP-YT",
  // .unit = "Status",
  // .topic = "XP/TW-01/DSP-YT-105", // Y-Status, T-Feedback/Transmit
  // .topicType = TAG_TOPIC_TYPE_PUBLISH,
  // .source = TAG_SOURCE_MODBUS,
  // .access = TAG_ACCESS_READ_ONLY,
  // .dataType = TAG_BOOL,
  // .sourceConfig = {
  //     .modbus = {
  //       .slaveId = 2, 
  //       .address = 20, 
  //       .quantity = 1, 
  //       .functionCode = 0x01, 
  //       .wordOrder = WORD_CDAB 
  //     }
  //   }
  // },
  {
  .tagId = 19,
  .name = "EOAP-YC",
  .unit = "Status",
  .topic = "XP/TW-01/EOAP-YC-111", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 21, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 20,
  .name = "EOAP_PLUS-YC",
  .unit = "Status",
  .topic = "XP/TW-01/EOAP_PLUS-111", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 22, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 21,
  .name = "EOAP_MINUS-YC",
  .unit = "Status",
  .topic = "XP/TW-01/EOAP_MINUS-111", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 23, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 22,
  .name = "MACH_PWR-YC",
  .unit = "Status",
  .topic = "XP/TW-01/MACH_PWR-YC-108", // Y-Status, C-Control
  .topicType = TAG_TOPIC_TYPE_SUBSCRIBE,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_WRITE_ONLY,
  .dataType = TAG_BOOL,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 24, 
        .quantity = 1, 
        .functionCode = 0x05, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 27,
  .name = "ECC_L1-YT",
  .unit = "Status",
  .topic = "XP/TW-01/ECC_L1-YT-107", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gHighLowMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 25, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 28,
  .name = "ECC_L2-YT",
  .unit = "Status",
  .topic = "XP/TW-01/ECC_L2-YT-107", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gHighLowMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 26, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 29,
  .name = "EOT_L1-YT",
  .unit = "Status",
  .topic = "XP/TW-01/EOT_L1-YT-107", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gHighLowMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 27, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 30,
  .name = "MACH-PWR-YT",
  .unit = "Status",
  .topic = "XP/TW-01/MACH-PWR-YT-108", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gOnOffMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 28, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 31,
  .name = "T-RSP-YT",
  .unit = "Status",
  .topic = "XP/TW-01/T-RSP-YT-109", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gOnOffMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 29, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 32,
  .name = "T-ECS-YT",
  .unit = "Status",
  .topic = "XP/TW-01/T-ECS-YT-110", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gOnOffMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 30, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 33,
  .name = "T-EOT-YT",
  .unit = "Status",
  .topic = "XP/TW-01/T-EOT-YT-111", // Y-Status, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_BOOL,
  .valueDataType = TAG_STRING,
  .boolMap = &gOnOffMap,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 31, 
        .quantity = 1, 
        .functionCode = 0x01, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 34,
  .name = "MACH-IT",
  .unit = "A",
  .topic = "XP/TW-01/MACH-IT-108", // I-Current, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .valueDataType = TAG_FLOAT32,
  .multiplier = 0.01f,
  .offset = 0.0f,
  .sourceConfig = {
      .modbus = {
        .slaveId = 3, 
        .address = 0, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 35,
  .name = "RSP-IT",
  .unit = "A",
  .topic = "XP/TW-01/RSP-IT-109", // I-Current, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .valueDataType = TAG_FLOAT32,
  .multiplier = 0.01f,
  .offset = 0.0f,
  .sourceConfig = {
      .modbus = {
        .slaveId = 3, 
        .address = 1, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 36,
  .name = "ECS-IT",
  .unit = "A",
  .topic = "XP/TW-01/ECS-IT-110", // I-Current, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .valueDataType = TAG_FLOAT32,
  .multiplier = 0.01f,
  .offset = 0.0f,
  .sourceConfig = {
      .modbus = {
        .slaveId = 3, 
        .address = 2, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 37,
  .name = "STR-IT",
  .unit = "A",
  .topic = "XP/TW-01/STR-IT-103", // I-Current, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .valueDataType = TAG_FLOAT32,
  .multiplier = 0.01f,
  .offset = 0.0f,
  .sourceConfig = {
      .modbus = {
        .slaveId = 3, 
        .address = 3, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 38,
  .name = "DSP-IT",
  .unit = "A",
  .topic = "XP/TW-01/DSP-IT-105", // I-Current, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .valueDataType = TAG_FLOAT32,
  .multiplier = 0.01f,
  .offset = 0.0f,
  .sourceConfig = {
      .modbus = {
        .slaveId = 3, 
        .address = 4, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 39,
  .name = "EOP-IT",
  .unit = "A",
  .topic = "XP/TW-01/EOP-IT-111", // I-Current, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .valueDataType = TAG_FLOAT32,
  .multiplier = 0.01f,
  .offset = 0.0f,
  .sourceConfig = {
      .modbus = {
        .slaveId = 3, 
        .address = 5, 
        .quantity = 1, 
        .functionCode = 0x04, 
        .wordOrder = WORD_CDAB 
      }
    }
  },
  {
  .tagId = 40,
  .name = "PROC-01-YT",
  .unit = "Status",
  .topic = "XP/TW-01/PROC-01-YT-112", // I-Current, T-Feedback/Transmit
  .topicType = TAG_TOPIC_TYPE_PUBLISH,
  .source = TAG_SOURCE_MODBUS,
  .access = TAG_ACCESS_READ_ONLY,
  .dataType = TAG_UINT16,
  .valueDataType = TAG_STRING,
  .enumMap = gProcessStateMap,
  .enumMapSize = 10,
  .sourceConfig = {
      .modbus = {
        .slaveId = 2, 
        .address = 0, 
        .quantity = 1, 
        .functionCode = 0x03, 
        .wordOrder = WORD_CDAB 
      }
    }
  }
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
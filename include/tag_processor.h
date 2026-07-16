#ifndef TAG_PROCESSOR_H
#define TAG_PROCESSOR_H

#include "tag_runtime.h"
#include "system_err.h"

/**
 * @brief Processes the raw hardware value stored in tag->rawValue and
 *        calculates the engineered value stored in tag->value.
 * 
 * Handles:
 *  1. Custom Boolean mappings (e.g. mapping true/false to ON/OFF or MANUAL/AUTO).
 *  2. Enum mappings (e.g. integer status registers to descriptive strings).
 *  3. Float scaling (multiplier * rawValue + offset).
 * 
 * @param tag Pointer to the runtime tag structure.
 * @return sys_status_t SYS_OK on success, error code on failure.
 */
sys_status_t tag_process(tag_runtime_t* tag);

#endif
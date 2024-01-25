/**
 * @file flash_controller.cpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @brief //TODO
 * @date 2023-09-17
 *
 * @copyright Copyright (c) 2023
 */

#include <esp_log.h>
#include <esp_partition.h>
#include <sdkconfig.h>

#include <efc_drivers/flash_controller.hpp>

static const uint32_t sector_size = 4096;
static inline uint32_t nearest_sector_address(uint32_t address) {
    return ((address / 4096) + 1) * 4096;             // HACK
    return (address + sector_size - 1) / sector_size; // ?? better
};

FlashController::FlashController() {
    auto iter = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
    uint32_t offset = 0;

    for (; iter != NULL; iter = esp_partition_next(iter))
        offset += esp_partition_get(iter)->size;

    ESP_LOGI("FLASH", "First offset: %d", (int)offset);

    // TODO Write __TIME__ to a header for the "non" partition
    // use this header to reload flash_structures

    // TODO Cloning sector(s) as power fail-safe update intermediate?
    // BUG Mutexting may be needed so that cloning sector doesnt fuck up in multicore env
    // BUG
}

// TODO Do not implement pop() / removal for flash_vector
// TODO Maybe call is flash_heap?

/**
 * @file flash_controller.hpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @author Michael Wigham (michael@wigham.net)
 * @brief //TODO
 * @date 2024-05-13
 *
 * @copyright Copyright (c) 2024
 */

#include <esp_flash_spi_init.h>
#include <esp_log.h>
#include <esp_partition.h>
#include <map>
#include <sdkconfig.h>

#ifndef EFC_FLASH_CONTROLLER_HPP
#define EFC_FLASH_CONTROLLER_HPP

#define TARGET_PARTITION_LABEL "data"

struct partitions_info_t {
    esp_partition_t *partition;
    uint32_t global_partition_head;
    uint32_t global_partition_capacity;
};

static std::map<std::string, partitions_info_t> m_partitions;

/**
 * @brief Class definition for FlashController
 */
template <typename inner_type_t>
class FlashController {
public:
    FlashController(uint32_t capacity, const char *label = "data") : m_capacity(capacity) {
        // Find partition to be used
        // BUG: no error handling if partition not found
        esp_partition_t *partition = (esp_partition_t *)esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, label);

        m_label = partition->label;
        m_erase_size = partition->erase_size;

        // If this is the first time that this partition is used, initialise the flash information
        if (m_partitions.find(m_label) == m_partitions.end()) {
            partitions_info_t p_info = {
                .partition = partition,
                .global_partition_head = partition->address,
                .global_partition_capacity = partition->size};

            m_partitions[m_label] = p_info;
        }

        // Claim flash region (Round to next sector so that there is no overlap between controllers)
        m_head_address = m_partitions[m_label].global_partition_head;
        m_tail_address = ((m_head_address + (sizeof(inner_type_t) * m_capacity) - 1 + m_erase_size) / m_erase_size) * m_erase_size;
        m_region_size = m_tail_address - m_head_address;

        m_partitions[m_label].global_partition_head += m_region_size;
        m_partitions[m_label].global_partition_capacity -= m_region_size;

        ESP_LOGW("FlashController", "Claimed Flash [ %lu | %lu | %lu ]", m_head_address, m_tail_address, m_region_size);
    }

    /**
     * @brief Performs a raw write operation, without checking if the region to be written has been previously flashed.
     * 
     * @param data Data to write
     * @param index Array index to write at
     */
    void unchecked_write(inner_type_t data, uint32_t index = 0) {
        if (index < m_capacity)
            esp_flash_write(NULL, &data, m_head_address + (index * sizeof(inner_type_t)), sizeof(inner_type_t));
    }

    /**
     * @brief Performs a raw read operation, without checking for encryption, valid data, or valid address.
     * 
     * @param index 
     * @return Read data 
     */
    inner_type_t unchecked_read(uint32_t index = 0) {
        inner_type_t read_value;
        esp_flash_read(NULL, &read_value, m_head_address + (index * sizeof(inner_type_t)), sizeof(inner_type_t));
        return read_value;
    }

    /**
     * @brief Erases all flash memory assosiated with this controller
     */
    void unchecked_erase() {
        esp_flash_erase_region(NULL, m_head_address, m_region_size);
    }

private:
    // Partition information
    const char *m_label;
    uint32_t m_erase_size;

    // Storage information
    uint32_t m_head_address;    // Points to the start of the data region
    uint32_t m_tail_address;    // Points to the end of the data region
    uint32_t m_region_size;     // Size of the data region in bytes
    uint32_t m_capacity;        // Total number of `inner_type_t`'s that can be stored
};

#endif // ifndef EFC_FLASH_CONTROLLER_HPP

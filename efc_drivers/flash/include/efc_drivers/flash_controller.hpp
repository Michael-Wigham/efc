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
    FlashController(uint32_t capacity, const char *label = "data") : m_mirror_address(0), m_capacity(capacity) {
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

        // Claim flash region
        m_head_address = m_partitions[m_label].global_partition_head;
        m_partitions[m_label].global_partition_head += m_capacity * sizeof(inner_type_t);
        m_partitions[m_label].global_partition_capacity -= m_capacity * sizeof(inner_type_t);
    }

    FlashController<inner_type_t> &operator[](uint32_t index) {
        m_mirror_address = m_head_address + (index * sizeof(inner_type_t));
        return *this;
    }

    inner_type_t &operator=(inner_type_t &data) {
        uint32_t erase_start = (m_mirror_address / m_erase_size) * m_erase_size;
        uint32_t erase_end = m_erase_size + (((m_mirror_address + sizeof(inner_type_t)) / m_erase_size) * m_erase_size);

        char *region_copy = new char[erase_end - erase_start];
        esp_flash_read(NULL, region_copy, erase_start, erase_end - erase_start);
        esp_flash_erase_region(NULL, erase_start, erase_end - erase_start);

        *(inner_type_t *)(region_copy + m_mirror_address - erase_start) = data;
        esp_flash_write(NULL, region_copy, erase_start, erase_end - erase_start);

        m_mirror_address = 0;

        delete[] region_copy;
        return data;
    }

    inner_type_t &operator=(inner_type_t &&data) {
        return operator=(data);
    }

    operator inner_type_t() {
        inner_type_t data;
        esp_flash_read(NULL, &data, m_mirror_address, sizeof(inner_type_t));

        m_mirror_address = 0;

        return data;
    }

private:
    // Partition information
    const char *m_label;
    uint32_t m_erase_size;

    uint32_t m_head_address; // Points to the start of the data partition

    // Storage information
    uint32_t m_mirror_address; // Points to the currently bound data address
    uint32_t m_capacity;       // Total number of `inner_type_t`'s that can be stored
};

#endif // ifndef EFC_FLASH_CONTROLLER_HPP

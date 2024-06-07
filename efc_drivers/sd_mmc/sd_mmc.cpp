#include <efc_drivers/sd_mmc.hpp>

SD_MMC::SD_MMC(gpio_num_t clk, gpio_num_t cmd, gpio_num_t d0, gpio_num_t d1, gpio_num_t d2, gpio_num_t d3) : m_clk(clk), m_cmd(cmd), m_d0(d0), m_d1(d1), m_d2(d2), m_d3(d3) {
}

bool SD_MMC::mount(const char *mountpoint, bool format_if_mount_failed) {
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.clk = m_clk;
    slot_config.cmd = m_cmd;
    slot_config.d0 = m_d0;
    slot_config.d1 = m_d1;
    slot_config.d2 = m_d2;
    slot_config.d3 = m_d3;
    slot_config.width = 4;

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.flags = SDMMC_HOST_FLAG_4BIT;
    host.slot = SDMMC_HOST_SLOT_1;
    host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;

    if (m_d1 == GPIO_NUM_NC || m_d2 == GPIO_NUM_NC || m_d3 == GPIO_NUM_NC) {
        slot_config.width = 1;
        host.flags = SDMMC_HOST_FLAG_1BIT;
    }

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = format_if_mount_failed,
        .max_files = 5,
        .allocation_unit_size = 0,
        .disk_status_check_enable = false,
        .use_one_fat = false};

    esp_err_t ret = esp_vfs_fat_sdmmc_mount(mountpoint, &host, &slot_config, &mount_config, &m_sd_card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE("SD_MMC", "Failed to mount filesystem.");
        } else if (ret == ESP_ERR_INVALID_STATE) {
            ESP_LOGE("SD_MMC", "SD Already mounted");
            return true;
        } else {
            ESP_LOGE("SD_MMC", "Failed to init card (0x%x).", ret);
        }
        m_sd_card = nullptr;
        return false;
    }
    return true;
}

sd_card_type SD_MMC::cardType() {
    if (!m_sd_card) {
        return sd_card_type::NONE;
    }
    if (m_sd_card->is_sdio) {
        return sd_card_type::SDIO;
    } else if (m_sd_card->is_mmc) {
        return sd_card_type::MMC;
    } else {
        return (m_sd_card->ocr & SD_OCR_SDHC_CAP) ? sd_card_type::SDHC : sd_card_type::SDSC;
    }
}

uint64_t SD_MMC::cardSize() {
    if (!m_sd_card) {
        return 0;
    }
    return (uint64_t)m_sd_card->csd.capacity * m_sd_card->csd.sector_size;
}

uint64_t SD_MMC::totalBytes() {
    FATFS *fsinfo;
    if (f_getfree("0:", nullptr, &fsinfo) != 0) {
        return 0;
    }
    uint64_t size = ((uint64_t)(fsinfo->csize)) * (fsinfo->n_fatent - 2);
#if FF_MAX_SS != FF_MIN_SS
    size = size * (fsinfo->ssize);
#endif
    return size;
}

uint64_t SD_MMC::usedBytes() {
    FATFS *fsinfo;
    if (f_getfree("0:", nullptr, &fsinfo) != 0) {
        return 0;
    }
    uint64_t size = ((uint64_t)(fsinfo->csize)) * ((fsinfo->n_fatent - 2) - (fsinfo->free_clst));
#if FF_MAX_SS != FF_MIN_SS
    size = size * (fsinfo->ssize);
#endif
    return size;
}

#define ONEMB (1024 * 1024)
#define ONEGB (1024 * 1024 * 1024)

char *fmtSize(uint64_t sizeVal) {
    // format size according to magnitude
    // only one call per format string
    static char returnStr[20];
    if (sizeVal < 50 * 1024) {
        sprintf(returnStr, "%llu bytes", sizeVal);
    } else if (sizeVal < ONEMB) {
        sprintf(returnStr, "%lluKB", sizeVal / 1024);
    } else if (sizeVal < ONEGB) {
        sprintf(returnStr, "%0.1fMB", (double)(sizeVal) / ONEMB);
    } else {
        sprintf(returnStr, "%0.1fGB", (double)(sizeVal) / ONEGB);
    }
    return returnStr;
}
/*
void SD_MMC::printSDInfo() {
    sd_card_type cardType = this->cardType();
    if (cardType == sd_card_type::NONE) {
        ESP_LOGW("SD_MMC", "No SD card attached");
    } else {
        const char *UNKNOWN = "UNKNOWN";
        const char *SD = "SD";
        const char *SDHC = "SDHC";
        char *typeStr;
        if (cardType == sd_card_type::SDIO) {
            typeStr = (char *)SD;
        } else if (cardType == sd_card_type::SDHC) {
            typeStr = (char *)SDHC;
        }
        printf("SD card type %s, Card Size: %s, Used Size: %s", typeStr, fmtSize(this->cardSize()), fmtSize(this->usedBytes()));
    }
}
 */
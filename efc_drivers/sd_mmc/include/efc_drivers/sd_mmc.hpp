// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef EFC_SD_MMC_HPP
#define EFC_SD_MMC_HPP

#include "sdkconfig.h"
#include "soc/soc_caps.h"
#ifdef SOC_SDMMC_HOST_SUPPORTED

#include <driver/sdmmc_defs.h>
#include <driver/sdmmc_host.h>
#include <esp_vfs_fat.h>
#include <sdmmc_cmd.h>
#include <soc/sdmmc_pins.h>

#include <soc/gpio_num.h>

#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#define SD_MMC_MNT "/sd_mmc"

enum class sd_card_type {
    NONE,
    MMC,
    SDIO,
    SDHC,
    SDSC,
    UNKNOWN
};

class SD_MMC {
public:
    SD_MMC(gpio_num_t clk, gpio_num_t cmd, gpio_num_t d0, gpio_num_t d1 = GPIO_NUM_NC, gpio_num_t d2 = GPIO_NUM_NC, gpio_num_t d3 = GPIO_NUM_NC);
    bool mount(const char *mountpoint = SD_MMC_MNT, bool format_if_mount_failed = false);
    sd_card_type cardType();
    uint64_t cardSize();
    uint64_t totalBytes();
    uint64_t usedBytes();

    void printSDInfo();
    sdmmc_card_t *m_sd_card;

private:
    gpio_num_t m_clk = GPIO_NUM_NC, m_cmd = GPIO_NUM_NC, m_d0 = GPIO_NUM_NC, m_d1 = GPIO_NUM_NC, m_d2 = GPIO_NUM_NC, m_d3 = GPIO_NUM_NC;
};

#endif // SOC_SDMMC_HOST_SUPPORTED

#endif // EFC_SD_MMC_HPP

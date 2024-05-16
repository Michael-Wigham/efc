/**
 * @file LIS3MDLTR.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-03-11
 *
 * @copyright Copyright (c) 2024
 */

#include <efc_devices/LIS3MDLTR.hpp>
#include <efc_drivers/i2c_device.hpp>

#include <array>
#include <memory>

LIS3MDLTR::LIS3MDLTR(uint8_t address) : I2CDevice(address) {
    auto [chip_id] = read_registers<int8_t, 1>(LIS3MDLTR_WHO_AM_I);
    if (chip_id != 0x3D) {
        ESP_LOGE("EFC_LIS3MDLTR", "Chip ID incorrect");
        return;
    }
    // Perform reset
    write_reg(LIS3MDLTR_CTRL_REG2, (BIT(2)));

    vTaskDelay(pdMS_TO_TICKS(10));

    write_reg(LIS3MDLTR_CTRL_REG1, (BIT(7)));

    // set high quality performance mode xy
    write_reg(LIS3MDLTR_CTRL_REG1, (LIS3MDLTR_ULTRAHIGHMODE << 5));
    write_reg(LIS3MDLTR_CTRL_REG4, (LIS3MDLTR_ULTRAHIGHMODE << 2));
    vTaskDelay(pdMS_TO_TICKS(10));

    write_reg(LIS3MDLTR_CTRL_REG1, (LIS3MDLTR_DATARATE_155_HZ << 1));

    write_reg(LIS3MDLTR_CTRL_REG2, (LIS3MDLTR_RANGE_4_GAUSS << 5));

    write_reg(LIS3MDLTR_CTRL_REG3, (LIS3MDLTR_CONTINUOUSMODE << 0));
}

int16_t LIS3MDLTR::read_temperature() {
    auto [temp] = read_registers<int16_t, 1>(LIS3MDLTR_TEMP_OUT_L);
    return temp;
}

std::array<int16_t, 3> LIS3MDLTR::mag_data() {
    return read_registers<int16_t, 3>(LIS3MDLTR_OUT_X_L);
}

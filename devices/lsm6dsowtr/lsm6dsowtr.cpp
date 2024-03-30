/**
 * @file lsm6dsowtr.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 */

#include <efc_devices/lsm6dsowtr.hpp>
#include <efc_drivers/i2c_device.hpp>

#include <array>
#include <memory>

LSM6DSOWTR::LSM6DSOWTR(uint8_t address) : I2CDevice(address) {
    // sw reset
    write_reg(CTRL3_C,  BIT(1));
    vTaskDelay(pdMS_TO_TICKS(10));

    write_reg(CTRL1_XL, (uint8_t)BIT(6));
    write_reg(CTRL2_G,  (uint8_t)BIT(6));
}

int16_t LSM6DSOWTR::read_temperature() {
    auto [temp] = read_registers<int16_t, 1>(OUT_TEMP_L);
    return temp;
}

std::array<int16_t, 3> LSM6DSOWTR::accel_data() {
    return read_registers<int16_t, 3>(OUTX_L_XL);
}

std::array<int16_t, 1> LSM6DSOWTR::accelx_data() {
    return read_registers<int16_t, 1>(OUTX_L_XL);
}

std::array<int16_t, 1> LSM6DSOWTR::accely_data() {
    return read_registers<int16_t, 1>(OUTY_L_XL);
}
std::array<int16_t, 1> LSM6DSOWTR::accelz_data() {
    return read_registers<int16_t, 1>(OUTZ_L_XL);
}

std::array<int16_t, 3> LSM6DSOWTR::gyro_data() {
    return read_registers<int16_t, 3>(OUTX_L_G);
}

std::array<int16_t, 1> LSM6DSOWTR::gyrox_data() {
    return read_registers<int16_t, 1>(OUTX_L_G);
}

std::array<int16_t, 1> LSM6DSOWTR::gyroy_data() {
    return read_registers<int16_t, 1>(OUTY_L_G);
}

std::array<int16_t, 1> LSM6DSOWTR::gyroz_data() {
    return read_registers<int16_t, 1>(OUTZ_L_G);
}
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
    write_reg(0x10, (uint8_t)0b01000000);
}

int16_t LSM6DSOWTR::read_temperature() {
    auto [temp] = read_registers<int16_t, 1>(0x20);
    return temp;
}

std::array<int16_t, 3> LSM6DSOWTR::accel_data() {
    return read_registers<int16_t, 3>(0x28);
}

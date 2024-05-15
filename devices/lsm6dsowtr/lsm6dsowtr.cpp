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
    // There was a SW_RESET here with a 10ms delay, its removal *may* have broken this sensor but all is seemingly okay
    // write_reg(CTRL3_C,  (uint8_t)BIT(0));
    write_reg(CTRL3_C,  (uint8_t)BIT(2));
    write_reg(CTRL1_XL, (uint8_t)BIT(6));
    write_reg(CTRL2_G,  (uint8_t)BIT(6));
}

// Temperature data
#define TO_CELCIUS(raw_temp) 25.0f + (raw_temp / 256.0f)
float LSM6DSOWTR::read_temperature() {
    auto [temp] = read_registers<int16_t, 1>(OUT_TEMP_L);
    return TO_CELCIUS(temp);
}

// Gyroscope data
#define TO_RADS2(raw_angular_accel) (raw_angular_accel / 32767.0f) * gyroscope_sensitivity * 0.017453f
std::array<float, 3> LSM6DSOWTR::read_gyroscope() {
    auto [raw_x, raw_y, raw_z] = read_registers<int16_t, 3>(OUTX_L_G);
    return {TO_RADS2(raw_x), TO_RADS2(raw_y), TO_RADS2(raw_z)};
}
float LSM6DSOWTR::read_gyroscope_x() {
    auto [data] = read_registers<int16_t, 1>(OUTX_L_G);
    return TO_RADS2(data);
}
float LSM6DSOWTR::read_gyroscope_y() {
    auto [data] = read_registers<int16_t, 1>(OUTX_L_G);
    return TO_RADS2(data);
}
float LSM6DSOWTR::read_gyroscope_z() {
    auto [data] = read_registers<int16_t, 1>(OUTX_L_G);
    return TO_RADS2(data);
}

// Accelerometer data
#define TO_MS2(raw_accel) (raw_accel / 32767.0f) * accelerometer_sensitivity
std::array<float, 3> LSM6DSOWTR::read_accelerometer() {
    auto [raw_x, raw_y, raw_z] = read_registers<int16_t, 3>(OUTX_L_XL);
    return {TO_MS2(raw_x), TO_MS2(raw_y), TO_MS2(raw_z)};
}
float LSM6DSOWTR::read_accelerometer_x() {
    auto [data] = read_registers<int16_t, 1>(OUTX_L_XL);
    return TO_MS2(data);
}
float LSM6DSOWTR::read_accelerometer_y() {
    auto [data] = read_registers<int16_t, 1>(OUTY_L_XL);
    return TO_MS2(data);
}
float LSM6DSOWTR::read_accelerometer_z() {
    auto [data] = read_registers<int16_t, 1>(OUTZ_L_XL);
    return TO_MS2(data);
}

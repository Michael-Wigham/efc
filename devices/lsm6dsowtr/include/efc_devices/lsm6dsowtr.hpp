/**
 * @file lsm6dsowtr.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_LSM6DSOWTR_HPP
#define EFC_LSM6DSOWTR_HPP

#include <efc_drivers/i2c_device.hpp>

#define LSM6DSOWTR_DEFAULT_ADRESS CONFIG_EFC_DEVICES_LSM6DSOWTR_SLAVE_ADDRESS

class LSM6DSOWTR : I2CDevice {
public:
    LSM6DSOWTR(uint8_t address = LSM6DSOWTR_DEFAULT_ADRESS);

    /**
     * @brief Read the temperature data from the device
     *
     * @return int16_t
     */
    int16_t read_temperature();

    /**
     * @brief Return x, y, z
     *
     * @return std::array<int16_t, 3>
     */
    std::array<int16_t, 3> accel_data();
};

#endif // ifndef EFC_LSM6DSOWTR_HPP

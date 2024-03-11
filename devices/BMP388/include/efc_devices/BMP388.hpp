/**
 * @file BMP388.hpp
 * @author Szymon Wlodarczyk (Szymon Wlodarczyk)
 * @brief // TODO:
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_BMP388_HPP
#define EFC_BMP388_HPP

#include <efc_drivers/i2c_device.hpp>

#define BMP388_DEFAULT_ADRESS CONFIG_EFC_DEVICES_BMP388_SLAVE_ADDRESS

class BMP388 : I2CDevice {
public:
    BMP388(uint8_t address = BMP388_DEFAULT_ADRESS) : I2CDevice(address) {
        /* Soft reset */
        write_reg(0x7E, 0xB6);

        /* Setup */
        auto [OSR_REG, ODR_REG, PWR_CTRL_REG] = read_registers<uint8_t, 3>(0x1B);
        write_reg(0x1B, PWR_CTRL_REG | 0b00110011);
        write_reg(0x1C, OSR_REG | 0b00000010);
        write_reg(0x1D, ODR_REG | 0b00000100);
    };

    float read_pressure() {
        auto [t0, t1, t2] = read_registers<uint8_t, 3>(0x04);
        int pressure = ((int)t0 << 16) + ((int)t1 << 8) + ((int)t2);
        return (float)pressure;
    }

private:

};

#endif // ifndef EFC_HCSR04_HPP

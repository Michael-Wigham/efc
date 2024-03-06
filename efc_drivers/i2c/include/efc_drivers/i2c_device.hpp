/**
 * @file i2c_device.hpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_I2C_DEVICE_HPP
#define EFC_I2C_DEVICE_HPP

#include "i2c_controller.hpp"

class I2CDevice {
public:
    I2CDevice(uint8_t device_address, uint8_t sda_pin = CONFIG_EFC_DRIVER_I2C_SDA, uint8_t scl_pin = CONFIG_EFC_DRIVER_I2C_SCL, i2c_port_t port = I2C_NUMBER(CONFIG_EFC_DRIVER_I2C_PORT_NUM), uint32_t frequency = CONFIG_EFC_DRIVER_I2C_FREQUENCY) : m_device_address(device_address) {
        m_controller = I2CController::get_i2c_controller(sda_pin, scl_pin, port, frequency);
    }

    I2CDevice(uint8_t device_address, I2CController *i2c_controller) : m_device_address(device_address) {
        m_controller = i2c_controller;
    }

    /**
     * @brief Return array of the requested data from the requested register
     *
     * @tparam OutputType
     * @tparam OutputCount
     * @param register_address
     * @return std::array<OutputType, OutputCount>
     */
    template <typename OutputType, int OutputCount>
    inline std::array<OutputType, OutputCount> read_registers(uint8_t register_address) {
        if (!m_controller) {
            return std::array<OutputType, OutputCount>{};
        }
        std::array<OutputType, OutputCount> output_bundle;
        uint8_t *output_pointer = reinterpret_cast<uint8_t *>(&output_bundle);
        int byte_count = sizeof(output_bundle);
        i2c_cmd_handle_t link = i2c_cmd_link_create();

        // Comm chain
        i2c_master_start(link);
        i2c_master_write_byte(link, (m_device_address << 1) | EFC_I2C_WRITE_BIT, EFC_I2C_CHECK_ACK);
        i2c_master_write_byte(link, register_address, EFC_I2C_CHECK_ACK);

        i2c_master_start(link);
        i2c_master_write_byte(link, (m_device_address << 1) | EFC_I2C_READ_BIT, EFC_I2C_CHECK_ACK);
        for (int offset = 0; offset < byte_count; offset++) {
            i2c_master_read_byte(link, output_pointer++, (offset == byte_count - 1) ? I2C_MASTER_NACK : I2C_MASTER_ACK);
        }
        i2c_master_stop(link);

        // Send commands to link
        esp_err_t ret = i2c_master_cmd_begin(m_controller->get_port(), link, 15 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(link);

        if (ret == ESP_FAIL)
            ESP_LOGE("I2C READ", "Slave did not acknowledge transfer");
        else if (ret == ESP_ERR_INVALID_STATE)
            ESP_LOGE("I2C READ", "Invalid I2C driver state");
        else if (ret == ESP_ERR_TIMEOUT)
            ESP_LOGE("I2C READ", "Timeout");

        return output_bundle;
    }

    /**
     * @brief Writes a byte of data to the given register on this device.
     *
     * @param register_address Register to write to
     * @param data Byte to write
     */
    bool write_reg(uint8_t register_address, uint8_t data) {
        if (!m_controller) {
            return false;
        }
        i2c_cmd_handle_t link = i2c_cmd_link_create();

        // Comm chain
        i2c_master_start(link);
        i2c_master_write_byte(link, (m_device_address << 1) | EFC_I2C_WRITE_BIT, EFC_I2C_CHECK_ACK);
        i2c_master_write_byte(link, register_address, EFC_I2C_CHECK_ACK);
        i2c_master_write_byte(link, data, EFC_I2C_CHECK_ACK);
        i2c_master_stop(link);

        // Send commands to link
        esp_err_t ret = i2c_master_cmd_begin(m_controller->get_port(), link, 15 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(link);

        if (ret == ESP_FAIL)
            ESP_LOGE("I2C WRITE", "Slave did not acknowledge transfer");
        else if (ret == ESP_ERR_INVALID_STATE)
            ESP_LOGE("I2C WRITE", "Invalid I2C driver state");
        else if (ret == ESP_ERR_TIMEOUT)
            ESP_LOGE("I2C WRITE", "Timeout");

        return true;
    }

    uint8_t getDeviceAddress() {
        return m_device_address;
    }

private:
    uint8_t m_device_address = 0x00;
    I2CController *m_controller = nullptr;
};

#endif // ifndef EFC_I2C_DEVICE_HPP

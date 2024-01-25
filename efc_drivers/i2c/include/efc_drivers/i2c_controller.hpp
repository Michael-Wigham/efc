/**
 * @file i2c_controller.hpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @author Michael Wigham (michael@wigham.net)
 * @brief //TODO
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_I2C_CONTROLLER_HPP
#define EFC_I2C_CONTROLLER_HPP

#include <driver/i2c.h>
#include <esp_log.h>

#include <array>
#include <memory>

#define EFC_I2C_WRITE_BIT I2C_MASTER_WRITE
#define EFC_I2C_READ_BIT I2C_MASTER_READ
#define EFC_I2C_CHECK_ACK 0x1
#define EFC_I2C_NO_CHECK_ACK 0x0
#define EFC_I2C_RX_BUFFER_SIZE 0
#define EFC_I2C_TX_BUFFER_SIZE 0

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

class I2CController {
public:
    /**
     * @brief Get i2c controller
     *
     * @param sda_pin
     * @param scl_pin
     * @param port (optional) default to I2C_NUM_0
     * @param frequency (optional) default to 100000
     * @return I2CController* or nullptr on error
     */
    static std::shared_ptr<I2CController> get_i2c_controller(uint8_t sda_pin = CONFIG_EFC_DRIVER_I2C_SDA, uint8_t scl_pin = CONFIG_EFC_DRIVER_I2C_SCL, i2c_port_t port = I2C_NUM_0, uint32_t frequency = CONFIG_EFC_DRIVER_I2C_FREQUENCY);

    /**
     * @brief Check if the device address is available on the bus
     *
     * @param address
     * @return esp_err_t
     */
    esp_err_t check_address(uint8_t address);

    /**
     * @brief print a list of all devices
     */
    void print_devices();

    explicit operator bool() const {
        return true;
    }

    /**
     * @brief Returns the configured sda pin
     *
     * @return uint8_t
     */
    inline uint8_t get_sda_pin() {
        return m_sda_pin;
    }

    /**
     * @brief Returns the configured scl pin
     *
     * @return uint8_t
     */
    inline uint8_t get_scl_pin() {
        return m_scl_pin;
    }

    /**
     * @brief Returns the configured port number
     *
     * @return uint8_t
     */
    inline i2c_port_t get_port() {
        return m_port;
    }

    /**
     * @brief Returns the configured i2c frequency
     *
     * @return uint8_t
     */
    inline uint32_t get_frequency() {
        return m_frequency;
    }

private:
    I2CController(uint8_t sda_pin, uint8_t scl_pin, i2c_port_t port, uint32_t frequency);

    uint8_t m_sda_pin;
    uint8_t m_scl_pin;
    i2c_port_t m_port;
    uint32_t m_frequency;

    inline static std::array<std::shared_ptr<I2CController>, I2C_NUM_MAX> m_instances;
};

#endif // ifndef EFC_I2C_CONTROLLER_HPP
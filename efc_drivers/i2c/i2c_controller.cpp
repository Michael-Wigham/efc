/**
 * @file i2c_controller.cpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <efc_drivers/i2c_controller.hpp>

I2CController::I2CController(uint8_t sda_pin, uint8_t scl_pin, i2c_port_t port, uint32_t frequency) : m_sda_pin(sda_pin), m_scl_pin(scl_pin), m_port(port), m_frequency(frequency) {
    // Configure I2C port
    i2c_config_t port_configuration = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {.clk_speed = frequency},
        .clk_flags = 0};

    if (i2c_driver_install(port, I2C_MODE_MASTER, EFC_I2C_RX_BUFFER_SIZE, EFC_I2C_TX_BUFFER_SIZE, 0) == ESP_ERR_INVALID_ARG) {
        ESP_LOGE("I2C Controller", "Failed to install I2C driver");
        return;
    };

    if (i2c_param_config(port, &port_configuration) == ESP_ERR_INVALID_ARG) {
        ESP_LOGE("I2C Controller", "Invalid port configuration");
        return;
    };
}

std::shared_ptr<I2CController> I2CController::get_i2c_controller(uint8_t sda_pin, uint8_t scl_pin, i2c_port_t port, uint32_t frequency) {
    if (port < I2C_NUM_0 || port >= I2C_NUM_MAX) {
        ESP_LOGE("I2C Controller", "Invalid port provided");
        return nullptr;
    }
    if (m_instances[port]) {
        uint8_t i_sda_pin = m_instances[port]->get_sda_pin();
        uint8_t i_scl_pin = m_instances[port]->get_scl_pin();
        uint32_t i_frequency = m_instances[port]->get_frequency();

        if (i_sda_pin == 0 || i_scl_pin == 0 || i_frequency == 0) {
            ESP_LOGE("I2C Controller", "Something is very wrong!");
            return nullptr;
        }
        else if (i_sda_pin == sda_pin && i_scl_pin == scl_pin && i_frequency == frequency) {
            return m_instances[port];
        }
        else {
            ESP_LOGW("I2C Controller", "Port selected taken, trying to auto select alternative");
            int available_port = -1;
            for (int i = I2C_NUM_0; i < I2C_NUM_MAX; i++) {
                ESP_LOGI("I2C Controller", "Checking port %d", i);
                if (m_instances[i] == nullptr) {
                    available_port = i;
                    break;
                }
            }
            if (available_port == -1) {
                for (int i = I2C_NUM_0; i < I2C_NUM_MAX; i++) {
                    ESP_LOGE("I2C Controller", "Port %d in use with settings:", i);
                    ESP_LOGE("I2C Controller", "\t- sda_pin %d", m_instances[i]->get_sda_pin());
                    ESP_LOGE("I2C Controller", "\t- scl_pin %d", m_instances[i]->get_scl_pin());
                    ESP_LOGE("I2C Controller", "\t- frequency %ld", m_instances[i]->get_frequency());
                }
                return nullptr;
            }
            else {
                ESP_LOGI("I2C Controller", "Port selected taken, using i2c port %d", available_port);
                m_instances[available_port] = std::make_shared<I2CController>(I2CController(sda_pin, scl_pin, (i2c_port_t)available_port, frequency));
                return m_instances[available_port];
            }
        }
        ESP_LOGI("I2C Controller", "Failed to get a controller");
        return nullptr;
    } else {
        m_instances[port] = std::make_shared<I2CController>(I2CController(sda_pin, scl_pin, port, frequency));
        return m_instances[port];
    }
}

esp_err_t I2CController::check_address(uint8_t address) {
    i2c_cmd_handle_t link = i2c_cmd_link_create();
    i2c_master_start(link);
    i2c_master_write_byte(link, (address << 1) | EFC_I2C_WRITE_BIT, EFC_I2C_CHECK_ACK);
    i2c_master_stop(link);
    esp_err_t ret = i2c_master_cmd_begin(m_port, link, 50 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(link);
    return ret;
}

void I2CController::print_devices() {
    uint8_t num_available = 0;
    ESP_LOGI("I2C Controller", "Available devices:");
    for (uint8_t address = 0; address < 128; address++) {
        if (check_address(address) == ESP_OK) {
            ESP_LOGI("I2C Controller", "\t- %02x", address);
            num_available++;
        }
    }
    if (num_available == 0) {
        ESP_LOGE("I2C Controller", "\tNONE");
    }
}
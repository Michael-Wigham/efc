/**
 * @file BMP388.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-03-18
 *
 * @copyright Copyright (c) 2024
 */

#include <efc_devices/BMP388.hpp>

BMP388::BMP388(uint8_t address) : I2CDevice(address) {
    // Soft reset
    write_reg(BMP388_REG_CMD, 0xB6);

    // 10ms delay
    ets_delay_us(10000);

    auto [chip_id] = read_registers<uint8_t, 1>(BMP388_REG_CHIP_ID);
    if (chip_id != 0x50) {
        // TODO: Chip id wrong should send error
    }

    // Setup
    auto [pwr_ctrl_reg] = read_registers<uint8_t, 1>(BMP388_REG_PWR_CTRL);
    write_reg(BMP388_REG_PWR_CTRL, pwr_ctrl_reg | BMP388_MODE_NORMAL_MODE | 0b00000011);
    set_pressure_oversampling(BMP388_OVERSAMPLING_x8);
    set_temperature_oversampling(BMP388_OVERSAMPLING_x1);
    set_odr(BMP388_ODR_50_HZ);

    auto [config_reg] = read_registers<uint8_t, 1>(BMP388_REG_CONFIG);
    write_reg(BMP388_REG_CONFIG, config_reg | 0b00000100);

    // Get Calibration data
    auto [t1] = read_registers<uint16_t, 1>(BMP388_REG_NVM_PAR_T1_L);
    m_t1 = t1;
    auto [t2] = read_registers<uint16_t, 1>(BMP388_REG_NVM_PAR_T2_L);
    m_t2 = t2;
    auto [t3] = read_registers<uint8_t, 1>(BMP388_REG_NVM_PAR_T3);
    m_t3 = (int8_t)t3;
    auto [p1] = read_registers<uint16_t, 1>(BMP388_REG_NVM_PAR_P1_L);
    m_p1 = p1;
    auto [p2] = read_registers<uint16_t, 1>(BMP388_REG_NVM_PAR_P2_L);
    m_p2 = p2;
    auto [p3] = read_registers<uint8_t, 1>(BMP388_REG_NVM_PAR_P3);
    m_p3 = (int8_t)p3;
    auto [p4] = read_registers<uint8_t, 1>(BMP388_REG_NVM_PAR_P4);
    m_p4 = (int8_t)p4;
    auto [p5] = read_registers<uint16_t, 1>(BMP388_REG_NVM_PAR_P5_L);
    m_p5 = p5;
    auto [p6] = read_registers<uint16_t, 1>(BMP388_REG_NVM_PAR_P6_L);
    m_p6 = p6;
    auto [p7] = read_registers<uint8_t, 1>(BMP388_REG_NVM_PAR_P7);
    m_p7 = (int8_t)p7;
    auto [p8] = read_registers<uint8_t, 1>(BMP388_REG_NVM_PAR_P8);
    m_p8 = (int8_t)p8;
    auto [p9] = read_registers<uint16_t, 1>(BMP388_REG_NVM_PAR_P9_L);
    m_p9 = p9;
    auto [p10] = read_registers<uint8_t, 1>(BMP388_REG_NVM_PAR_P10);
    m_p10 = (int8_t)p10;
    auto [p11] = read_registers<uint8_t, 1>(BMP388_REG_NVM_PAR_P11);
    m_p11 = (int8_t)p11;
};

bmp388_odr_t BMP388::get_odr() {
    auto [odr_reg] = read_registers<uint8_t, 1>(BMP388_REG_ODR);

    return (bmp388_odr_t)((odr_reg >> 0) & 31);
}

void BMP388::set_odr(bmp388_odr_t odr) {
    auto [odr_reg] = read_registers<uint8_t, 1>(BMP388_REG_ODR);

    odr_reg &= ~(0b00011111 << 0);
    odr_reg |= odr << 0;
    write_reg(BMP388_REG_ODR, odr_reg);
}

bmp388_oversampling_t BMP388::get_pressure_oversampling() {
    auto [osr_reg] = read_registers<uint8_t, 1>(BMP388_REG_OSR);

    return (bmp388_oversampling_t)((osr_reg >> 0) & 0x07);
}

void BMP388::set_pressure_oversampling(bmp388_oversampling_t oversampling) {
    auto [prev_osr_reg] = read_registers<uint8_t, 1>(BMP388_REG_OSR);

    prev_osr_reg &= ~(7 << 0);
    prev_osr_reg |= oversampling << 0;
    write_reg(BMP388_REG_OSR, prev_osr_reg);
}

bmp388_oversampling_t BMP388::get_temperature_oversampling() {
    auto [osr_reg] = read_registers<uint8_t, 1>(BMP388_REG_OSR);

    return (bmp388_oversampling_t)((osr_reg >> 3) & 0x07);
}

void BMP388::set_temperature_oversampling(bmp388_oversampling_t oversampling) {
    auto [prev_osr_reg] = read_registers<uint8_t, 1>(BMP388_REG_OSR);

    prev_osr_reg &= ~(7 << 3);
    prev_osr_reg |= oversampling << 3;
    write_reg(BMP388_REG_OSR, prev_osr_reg);
}

float BMP388::get_temperature() {
    auto [fifo_cfg1] = read_registers<uint8_t, 1>(BMP388_REG_FIFO_CONFIG_1);
    if (((fifo_cfg1 & BIT(0)) != 0) || ((fifo_cfg1 & BIT(4)) != 0)) {
        ESP_LOGE("BMP388", "fifo mode disabled or no temperature data is stored");
        return 0;
    }
    auto [pwr_ctrl_reg] = read_registers<uint8_t, 1>(BMP388_REG_PWR_CTRL);
    if (((pwr_ctrl_reg >> 4) & 0x03) == 0x03) {
        auto [status_reg] = read_registers<uint8_t, 1>(BMP388_REG_STATUS);
        if ((status_reg & (1 << 6)) != 0) {
            int64_t output;

            auto [data3_reg, data4_reg, data5_reg] = read_registers<uint8_t, 3>(BMP388_REG_DATA_3);
            uint32_t raw_data = (uint32_t)data5_reg << 16 | (uint32_t)data4_reg << 8 | data3_reg;
            output = _comp_temperature(raw_data);
            return (float)((double)output / 100.0);
        }
        ESP_LOGI("BMP388", "Temperature data not ready to be read");
        return 0;
    } else if (((fifo_cfg1 >> 4) & 0x03) == 0x00) {
        uint16_t cnt = 5000;

        pwr_ctrl_reg &= ~(0x03 << 4); /* clear 4-5 bits */
        pwr_ctrl_reg |= 0x01 << 4;    /* set bit 4 */
        write_reg(BMP388_REG_PWR_CTRL, pwr_ctrl_reg);

        while (1) {
            auto [status_reg] = read_registers<uint8_t, 1>(BMP388_REG_STATUS);
            if ((status_reg & (1 << 6)) != 0) {
                int64_t output;

                auto [data3_reg, data4_reg, data5_reg] = read_registers<uint8_t, 3>(BMP388_REG_DATA_3);
                uint32_t raw_data = (uint32_t)data5_reg << 16 | (uint32_t)data4_reg << 8 | data3_reg;
                output = _comp_temperature(raw_data);
                return (float)((double)output / 100.0);
            } else {
                if (cnt != 0) {
                    cnt--;
                    ets_delay_us(1000);
                    continue;
                }
                ESP_LOGI("BMP388", "Temperature data not ready to be read");
                return 0;
            }
        }
    } else {
        ESP_LOGI("BMP388", "mode is invalid");
        return 0;
    }
}

float BMP388::get_pressure() {
    uint32_t temperature_yaw;

    auto [fifo_cfg1] = read_registers<uint8_t, 1>(BMP388_REG_FIFO_CONFIG_1);
    if (((fifo_cfg1 & BIT(0)) != 0) || ((fifo_cfg1 & BIT(4)) != 0) || ((fifo_cfg1 & BIT(3)) != 0)) {
        ESP_LOGE("BMP388", "fifo mode disabled or no temperature or pressure data is stored");
        return 0;
    }
    auto [pwr_ctrl_reg] = read_registers<uint8_t, 1>(BMP388_REG_PWR_CTRL);
    if (((pwr_ctrl_reg >> 4) & 0x03) == 0x03) {
        auto [status_reg] = read_registers<uint8_t, 1>(BMP388_REG_STATUS);
        if ((status_reg & (1 << 6)) != 0) {
            auto [data3_reg, data4_reg, data5_reg] = read_registers<uint8_t, 3>(BMP388_REG_DATA_3);
            temperature_yaw = (uint32_t)data5_reg << 16 | (uint32_t)data4_reg << 8 | data3_reg;
            (void)_comp_temperature(temperature_yaw);
        } else {
            ESP_LOGI("BMP388", "Temperature data not ready to be read");
            return 0;
        }
        if ((status_reg & (1 << 5)) != 0) {
            int64_t output;

            auto [data0_reg, data1_reg, data2_reg] = read_registers<uint8_t, 3>(BMP388_REG_DATA_0);
            ESP_LOGI("BMP388", "PRESS data %d %d %d", data2_reg, data1_reg, data0_reg);
            uint32_t data = (uint32_t)data2_reg << 16 | (uint32_t)data1_reg << 8 | data0_reg;
            output = _comp_pressure(data);
            return (float)((double)output / 100.0);
        } else {
            ESP_LOGI("BMP388", "Pressure data not ready to be read");
            return 0;
        }
    } else if (((pwr_ctrl_reg >> 4) & 0x03) == 0x00) {
        uint16_t cnt = 5000;

        pwr_ctrl_reg &= ~(0x03 << 4);
        pwr_ctrl_reg |= 0x01 << 4;
        write_reg(BMP388_REG_PWR_CTRL, pwr_ctrl_reg);

        while (1) {
            auto [status_reg] = read_registers<uint8_t, 1>(BMP388_REG_STATUS);
            if ((status_reg & (1 << 6)) != 0) {
                auto [data3_reg, data4_reg, data5_reg] = read_registers<uint8_t, 3>(BMP388_REG_DATA_3);
                temperature_yaw = (uint32_t)data5_reg << 16 | (uint32_t)data4_reg << 8 | data3_reg;
                (void)_comp_temperature(temperature_yaw);
            } else {
                if (cnt != 0) {
                    cnt--;
                    ets_delay_us(1000);
                    continue;
                }
                ESP_LOGI("BMP388", "Temperature data not ready to be read");
                return 0;
            }
            cnt = 5000;
            if ((status_reg & (1 << 5)) != 0) {
                int64_t output;

                auto [data0_reg, data1_reg, data2_reg] = read_registers<uint8_t, 3>(BMP388_REG_DATA_0);
                uint32_t data = (uint32_t)data2_reg << 16 | (uint32_t)data1_reg << 8 | data0_reg;
                output = _comp_pressure(data);
                return (float)((double)output / 100.0);
            } else {
                if (cnt != 0) {
                    cnt--;
                    ets_delay_us(1000);
                    continue;
                }
                ESP_LOGI("BMP388", "Temperature data not ready to be read");
                return 0;
            }
        }
    }
    ESP_LOGI("BMP388", "mode is invalid");
    return 0;
}

int64_t BMP388::_comp_temperature(uint32_t data) {
    uint64_t pd1;
    uint64_t pd2;
    uint64_t pd3;
    int64_t pd4;
    int64_t pd5;
    int64_t pd6;
    int64_t ct;

    /* calculate compensate temperature */
    pd1 = (uint64_t)(data - (256 * (uint64_t)(m_t1)));
    pd2 = (uint64_t)(m_t2 * pd1);
    pd3 = (uint64_t)(pd1 * pd1);
    pd4 = (int64_t)(((int64_t)pd3) * ((int64_t)m_t3));
    pd5 = ((int64_t)(((int64_t)pd2) * 262144) + (int64_t)pd4);
    pd6 = (int64_t)(((int64_t)pd5) / 4294967296U);
    m_t_fine = pd6;
    ct = (int64_t)((pd6 * 25) / 16384);

    return ct;
}

int64_t BMP388::_comp_pressure(uint32_t data) {
    int64_t pd1;
    int64_t pd2;
    int64_t pd3;
    int64_t pd4;
    int64_t pd5;
    int64_t pd6;
    int64_t offset;
    int64_t sensitivity;
    uint64_t comp_press;

    /* calculate compensate pressure */
    pd1 = m_t_fine * m_t_fine;
    pd2 = pd1 / 64;
    pd3 = (pd2 * m_t_fine) / 256;
    pd4 = (m_p8 * pd3) / 32;
    pd5 = (m_p7 * pd1) * 16;
    pd6 = (m_p6 * m_t_fine) * 4194304;
    offset = (int64_t)((int64_t)(m_p5) * (int64_t)140737488355328U) + pd4 + pd5 + pd6;
    pd2 = (((int64_t)m_p4) * pd3) / 32;
    pd4 = (m_p3 * pd1) * 4;
    pd5 = ((int64_t)(m_p2)-16384) * ((int64_t)m_t_fine) * 2097152;
    sensitivity = (((int64_t)(m_p1)-16384) * (int64_t)70368744177664U) + pd2 + pd4 + pd5;
    pd1 = (sensitivity / 16777216) * data;
    pd2 = (int64_t)(m_p10) * (int64_t)(m_t_fine);
    pd3 = pd2 + (65536 * (int64_t)(m_p9));
    pd4 = (pd3 * data) / 8192;
    pd5 = (pd4 * data) / 512;
    pd6 = (int64_t)((uint64_t)data * (uint64_t)data);
    pd2 = ((int64_t)(m_p11) * (int64_t)(pd6)) / 65536;
    pd3 = (pd2 * data) / 128;
    pd4 = (offset / 4) + pd1 + pd5 + pd3;
    comp_press = (((uint64_t)pd4 * 25) / (uint64_t)1099511627776U);

    return comp_press;
}

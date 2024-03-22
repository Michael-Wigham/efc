/**
 * @file TCS3472X.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief //TODO
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 */

#include <efc_devices/TCS3472X.hpp>

#include <efc_common/common.hpp>

TCS3472X::TCS3472X(uint8_t address, uint8_t integration_time, tcs3472X_gain_t gain) : I2CDevice(address), m_integration_time(integration_time), m_gain(gain) {
    auto [part_id] = read_registers<uint8_t, 1>(TCS3472X_ID);
    if ((part_id != 0x4D) && (part_id != 0x44)) {
        // TODO: Should probably set a thing
        return;
    }
    setIntegrationTime(m_integration_time);
    setGain(m_gain);

    enable();
}

void TCS3472X::enable() {
    write_reg(TCS3472X_ENABLE, TCS3472X_ENABLE_PON);
    delay(10);
    write_reg(TCS3472X_ENABLE, TCS3472X_ENABLE_PON | TCS3472X_ENABLE_AEN);
    // Set a delay for the integration time
    delay((256 - m_integration_time) * 12 / 5 + 1);
}

void TCS3472X::disable() {
    /* Turn the device off to save power */
    auto [reg] = read_registers<uint8_t, 1>(TCS3472X_ENABLE);
    write_reg(TCS3472X_ENABLE, reg & ~(TCS3472X_ENABLE_PON | TCS3472X_ENABLE_AEN));
}

void TCS3472X::setIntegrationTime(uint8_t integration_time) {
    /* Update the timing register */
    write_reg(TCS3472X_TIMING, integration_time);

    m_integration_time = integration_time;
}

void TCS3472X::setGain(tcs3472X_gain_t gain) {
    /* Update the control register */
    write_reg(TCS3472X_CONTROL, gain);

    m_gain = gain;
}

void TCS3472X::getRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {

    auto [_c] = read_registers<uint16_t, 1>(TCS3472X_CDATAL);
    *c = _c;
    auto [_r] = read_registers<uint16_t, 1>(TCS3472X_RDATAL);
    *r = _r;
    auto [_g] = read_registers<uint16_t, 1>(TCS3472X_GDATAL);
    *g = _g;
    auto [_b] = read_registers<uint16_t, 1>(TCS3472X_BDATAL);
    *b = _b;

    // Set a delay for the integration time
    delay(((256 - m_integration_time) * (12 / 5)) + 1);
}

void TCS3472X::getRawDataOneShot(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
    enable();
    getRawData(r, g, b, c);
    disable();
}

void TCS3472X::getRGB(float *r, float *g, float *b) {
    uint16_t red, green, blue, clear;
    getRawData(&red, &green, &blue, &clear);
    uint32_t sum = clear;

    if (clear == 0) {
        *r = *g = *b = 0;
        return;
    }

    *r = ((float)red * 255.0) / sum;
    *g = ((float)green * 255.0) / sum;
    *b = ((float)blue * 255.0) / sum;
}

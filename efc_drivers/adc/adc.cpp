/**
 * @file adc.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 */

#include <efc_drivers/adc.hpp>

#include <inttypes.h>

ADC *ADC::getADC(adc_pins_t pin) {
    if (m_instances[pin]) {
        uint32_t i_gpio_pin = m_instances[pin]->getPin();

        if (i_gpio_pin == 0) {
            return nullptr;
        } else if (i_gpio_pin == pin) {
            return m_instances[pin];
        }
        return nullptr;
    } else {
        adc_channel_t channel;
        adc_unit_t adc_unit;

        esp_err_t err = ESP_OK;
        err = adc_oneshot_io_to_channel(pin, &adc_unit, &channel);
        if (err != ESP_OK) {
            ESP_LOGE("EFC_ADC", "Pin %" PRIu32 " does not support ADC", (uint32_t)pin);
        }
        ESP_LOGI("EFC_ADC", "INFO UNIT %" PRIu32 ", Channel %" PRIu32, (uint32_t)adc_unit + 1, (uint32_t)channel);
        m_instances[pin] = new ADC(pin, adc_unit, channel);
        return m_instances[pin];
    }
}

ADC::ADC(adc_pins_t pin, adc_unit_t adc_unit, adc_channel_t channel) : m_pin(pin), m_adc_unit(adc_unit), m_channel(channel) {}

uint16_t ADC::read() {
    esp_err_t err;
    int value = 0;

    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT};

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = m_adc_unit,
        .ulp_mode = ADC_ULP_MODE_DISABLE};

    err = adc_oneshot_new_unit(&init_config, &m_adc_oneshot_handle);
    if (err != ESP_OK) {
        return 0;
    }

    err = adc_oneshot_config_channel(m_adc_oneshot_handle, m_channel, &chan_config);
    if (err != ESP_OK) {
        return 0;
    }

    err = adc_oneshot_read(m_adc_oneshot_handle, m_channel, &value);
    if (err != ESP_OK) {
        return 0;
    }

    err = adc_oneshot_del_unit(m_adc_oneshot_handle);
    if (err != ESP_OK) {
        return 0;
    }

    return value;
}

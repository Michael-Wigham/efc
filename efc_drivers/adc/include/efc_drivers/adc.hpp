/**
 * @file adc.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief //TODO
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_ADC_HPP
#define EFC_ADC_HPP

#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_log.h>

#include <soc/adc_channel.h>

#include <array>
#include <memory>
#include <sys/time.h>

#define xstr(a, b) str(a, b)
#define str(a, b) a##b

enum adc_pins_t {
#ifdef ADC1_CHANNEL_0_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_0_GPIO_NUM) = ADC1_CHANNEL_0_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_1_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_1_GPIO_NUM) = ADC1_CHANNEL_1_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_2_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_2_GPIO_NUM) = ADC1_CHANNEL_2_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_3_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_3_GPIO_NUM) = ADC1_CHANNEL_3_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_4_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_4_GPIO_NUM) = ADC1_CHANNEL_4_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_5_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_5_GPIO_NUM) = ADC1_CHANNEL_5_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_6_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_6_GPIO_NUM) = ADC1_CHANNEL_6_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_7_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_7_GPIO_NUM) = ADC1_CHANNEL_7_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_8_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_8_GPIO_NUM) = ADC1_CHANNEL_8_GPIO_NUM,
#endif
#ifdef ADC1_CHANNEL_9_GPIO_NUM
    xstr(GPIO_, ADC1_CHANNEL_9_GPIO_NUM) = ADC1_CHANNEL_9_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_0_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_0_GPIO_NUM) = ADC2_CHANNEL_0_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_1_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_1_GPIO_NUM) = ADC2_CHANNEL_1_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_2_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_2_GPIO_NUM) = ADC2_CHANNEL_2_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_3_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_3_GPIO_NUM) = ADC2_CHANNEL_3_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_4_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_4_GPIO_NUM) = ADC2_CHANNEL_4_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_5_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_5_GPIO_NUM) = ADC2_CHANNEL_5_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_6_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_6_GPIO_NUM) = ADC2_CHANNEL_6_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_7_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_7_GPIO_NUM) = ADC2_CHANNEL_7_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_8_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_8_GPIO_NUM) = ADC2_CHANNEL_8_GPIO_NUM,
#endif
#ifdef ADC2_CHANNEL_9_GPIO_NUM
    xstr(GPIO_, ADC2_CHANNEL_9_GPIO_NUM) = ADC2_CHANNEL_9_GPIO_NUM,
#endif
};

class ADC {
public:
    static ADC *getADC(adc_pins_t pin);

    ~ADC();

    uint16_t read();

    adc_pins_t getPin() __attribute__((__noinline__)) {
        return m_pin;
    }

    explicit operator bool() const {
        return true;
    }

private:
    ADC(adc_pins_t pin, adc_unit_t adc_unit, adc_channel_t channel);

    adc_pins_t m_pin;
    adc_unit_t m_adc_unit;
    adc_channel_t m_channel;
    adc_oneshot_unit_handle_t m_adc_oneshot_handle = nullptr;

    // FIXME: GPIO_NUM_MAX is incorrect for the esp32s3 there are only 20 and the esp32 only has 18 etc...
    inline static std::array<ADC *, GPIO_NUM_MAX> m_instances;
};

#endif // EFC_ADC_HPP

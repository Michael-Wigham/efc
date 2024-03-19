/**
 * @file pwm_controller.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-03-14
 *
 * @copyright Copyright (c) 2024
 */
#ifndef EFC_PWM_CONTROLLER_HPP
#define EFC_PWM_CONTROLLER_HPP

#include <driver/gpio.h>
#include <driver/ledc.h>
#include <esp_err.h>
#include <esp_log.h>

#include <array>

class PWMController {
public:
    static PWMController *get_pwm_controller(gpio_num_t gpio);

    void set_duty(uint32_t duty);

    uint32_t get_duty();

    explicit operator bool() const {
        return true;
    }

private:
    PWMController(gpio_num_t gpio, ledc_channel_t channel);
    inline static std::array<PWMController *, LEDC_CHANNEL_MAX> m_instances;

    gpio_num_t m_gpio;
    ledc_channel_t m_channel;
};

#endif // EFC_PWM_CONTROLLER_HPP

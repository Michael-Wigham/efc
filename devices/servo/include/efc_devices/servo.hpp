/**
 * @file servos.hpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @brief
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EFC_SERVOS_HPP
#define EFC_SERVOS_HPP

#include <efc_drivers/pwm_controller.hpp>

/**
 * @brief
 */
class Servo {
public:
    /**
     * @brief Construct a new Servo object
     *
     * @param pin
     * @param angle_min
     * @param angle_max
     */
    Servo(int pin, int angle_min = 0, int angle_max = 0) {
        // Create PWM comparator and generator
        PWMController m_servo_controller;
        std::tie(m_comparator, m_generator) = m_servo_controller.create_config_for_gpio(pin);
    }

    void write(int value) {
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(m_generator, MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
        mcpwm_comparator_set_compare_value(m_comparator, value);
    }

    void disable() {
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(m_generator, MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_LOW)));
    }

private:
    // Instance state
    mcpwm_cmpr_handle_t m_comparator;
    mcpwm_gen_handle_t m_generator;
};

#endif // ifndef EFC_SERVOS_HPP

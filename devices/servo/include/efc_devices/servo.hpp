/**
 * @file servo.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-03-15
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_SERVOS_HPP
#define EFC_SERVOS_HPP

#include <efc_drivers/pwm_controller.hpp>

#define calc_duty(angle, max_angle, max_width, min_width, freq) \
    (uint32_t)((float)m_full_duty * ((float)angle / max_angle * (max_width - min_width) + min_width) * freq / (1000000.0f))

#define calc_angle(duty, max_angle, max_width, min_width, freq) \
    ((float)(((float)duty * ((1000000.0f / (float)m_full_duty) / freq)) - min_width) * (max_angle / (max_width - min_width)))

/**
 * @brief Servo controller using PWMController
 */
class Servo {
public:
    /**
     * @brief Construct a new Servo object
     *
     * @param pin
     */
    Servo(gpio_num_t gpio) {
        m_servo_controller = PWMController::get_pwm_controller(gpio);
    }

    /**
     * @brief Set the min and max angle that the servo should move
     *
     * @param angle_min
     * @param angle_max
     */
    void set_min_max_angle(int angle_min, int angle_max) {
        m_angle_min = angle_min;
        m_angle_max = angle_max;
    }

    /**
     * @brief Set the min max pwm signal
     *
     * @param width_min
     * @param width_max
     */
    void set_min_max_width(int width_min, int width_max) {
        m_width_min = width_min;
        m_width_max = width_max;
    }

    /**
     * @brief write an angle to the servo
     *
     * @param angle
     */
    void write(int angle) {
        if (angle < m_angle_min) {
            angle = m_angle_min;
        }
        m_servo_controller->set_duty(calc_duty(angle, m_angle_max, m_width_max, m_width_min, 50));
    }

    /**
     * @brief read the angle being sent to the servo
     *
     * @return uint32_t
     */
    float read() {
        return calc_angle(m_servo_controller->get_duty(), m_angle_max, m_width_max, m_width_min, 50);
    }

    void disable() {
        delete m_servo_controller;
    }

private:
    PWMController *m_servo_controller;

    uint32_t m_angle_min = 0, m_angle_max = 180;
    uint32_t m_width_min = 500, m_width_max = 2500;
    uint32_t m_full_duty = (1 << LEDC_TIMER_10_BIT) - 1;
};

#endif // ifndef EFC_SERVOS_HPP

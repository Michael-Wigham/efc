/**
 * @file HCSR04.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024
 */

#include <efc_devices/HCSR04.hpp>

#include <array>

HCSR04::HCSR04(gpio_num_t trigger_pin, gpio_num_t echo_pin) {
    m_trigger = GPIO::getGPIO(trigger_pin, OUTPUT);
    m_echo = GPIO::getGPIO(echo_pin, INPUT);
}

double HCSR04::dist() const {
    m_trigger->write(LOW);
    ets_delay_us(2);
    m_trigger->write(HIGH);
    ets_delay_us(10);
    m_trigger->write(LOW);
    portDISABLE_INTERRUPTS();
    double distance_cm = m_echo->pulseIn(HIGH) * 0.034 / 2;
    portENABLE_INTERRUPTS();
    return distance_cm;
}

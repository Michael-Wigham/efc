/**
 * @file HCSR04.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024
 */

#include <efc_devices/HCSR04.hpp>

#include <esp_timer.h>

#include <array>

void delay(uint32_t delay) {
    const int64_t start = esp_timer_get_time();
    if (delay) {
        int64_t end = (start + delay);
        if (start > end) {
            while (esp_timer_get_time() > end) {
                asm volatile("nop");
            }
        }
        while (esp_timer_get_time() < end) {
            asm volatile("nop");
        }
    }
}

HCSR04::HCSR04(uint32_t trigger_pin, uint32_t echo_pin) {
    m_trigger = GPIO::getGPIO(trigger_pin, OUTPUT);
    m_echo = GPIO::getGPIO(echo_pin, INPUT);
}

double HCSR04::dist() const {
    m_trigger->write(LOW);
    delay(2);
    m_trigger->write(HIGH);
    delay(10);
    m_trigger->write(LOW);
    portDISABLE_INTERRUPTS();
    double distance_cm = m_echo->pulseIn(HIGH) / 58.8235;
    portENABLE_INTERRUPTS();
    return distance_cm;
}

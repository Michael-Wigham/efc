/**
 * @file HCSR04.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_HCSR04_HPP
#define EFC_HCSR04_HPP

#include <efc_drivers/gpio.hpp>

#include <soc/gpio_num.h>

#include <memory>

class HCSR04 {
public:
    /**
     * @brief Create HCSR04 device
     *
     * @param trigger_pin
     * @param echo_pin
     */
    HCSR04(gpio_num_t trigger_pin, gpio_num_t echo_pin);

    /**
     * @brief Returns distance in cm
     *
     * @return double
     */
    double dist() const;

private:
    std::shared_ptr<GPIO> m_trigger = nullptr;
    std::shared_ptr<GPIO> m_echo = nullptr;
};

#endif // ifndef EFC_HCSR04_HPP

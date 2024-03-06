/**
 * @file gpio.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief //TODO
 * @date 2024-01-30
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_GPIO_CONTROLLER_HPP
#define EFC_GPIO_CONTROLLER_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include <driver/gpio.h>
#include <esp_log.h>

#include <soc/gpio_num.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <array>
#include <memory>
#include <sys/time.h>

#define PULLUP (1 << 2)
#define PULLDOWN (1 << 3)
#define OPEN_DRAIN (1 << 4)

typedef enum {
    GPIO_DISABLE = 0x0,
    INPUT = (1 << 0),
    OUTPUT = (1 << 1),
    INPUT_OUTPUT = (INPUT | OUTPUT),
    INPUT_PULLUP = (INPUT | PULLUP),
    INPUT_PULLDOWN = (INPUT | PULLDOWN),
    OUTPUT_OD = (OUTPUT | OPEN_DRAIN),
    INPUT_OUTPUT_OD = (INPUT_OUTPUT | OPEN_DRAIN)
} gpio_pinmode_t;

typedef enum {
    INTR_DISABLE,
    POSEDGE,
    NEGEDGE,
    ANYEDGE
} gpio_intr_mode_t;

typedef enum {
    LOW = 0,
    HIGH = 1
} gpio_state_t;

class GPIO {
public:
    static std::shared_ptr<GPIO> getGPIO(gpio_num_t pin, gpio_pinmode_t pin_mode/* , gpio_intr_mode_t intr = INTR_DISABLE */);

    gpio_state_t read();

    bool write(gpio_state_t value);

    /**
     * @brief Return duration of a state in us
     *
     * @param pin
     * @param state
     * @param timeout
     * @return double
     */
    double pulseIn(gpio_state_t state, unsigned long timeout = 20000);

    gpio_num_t getPin() __attribute__ ((__noinline__)) {
        return m_pin;
    }

    gpio_pinmode_t getPinMode() __attribute__ ((__noinline__)) {
        return m_pin_mode;
    }

    explicit operator bool() const {
        return true;
    }

private:
    GPIO(gpio_num_t pin, gpio_pinmode_t pin_mode/* , gpio_intr_mode_t intr = INTR_DISABLE */);

    gpio_num_t m_pin = GPIO_NUM_NC;
    gpio_pinmode_t m_pin_mode;

    /* inline static int isr_running = 0; */

    inline static std::array<std::shared_ptr<GPIO>, GPIO_NUM_MAX> m_instances;
};

#endif // define EFC_GPIO_CONTROLLER_HPP

/**
 * @file gpio.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-01-30
 *
 * @copyright Copyright (c) 2024
 */

#include <efc_drivers/gpio.hpp>
#include <esp_cpu.h>
#include <esp_system.h>
#include <soc/rtc.h>

#define WAIT_FOR_PIN_STATE(state)                                             \
    while (read() != (state)) {                                               \
        if (esp_cpu_get_cycle_count() - start_cycle_count > timeout_cycles) { \
            return 0;                                                         \
        }                                                                     \
    }

/*
static QueueHandle_t gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

static void IRAM_ATTR gpio_isr_handler(void *arg) {
    uint32_t pin = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &pin, NULL);
}

void gpio_isr_task(void *arg) {
    uint32_t pin;
    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &pin, portMAX_DELAY)) {
            std::shared_ptr<GPIO> gpio = GPIO::getGPIO(pin, INPUT);
            if (!gpio) {
                gpio = GPIO::getGPIO(pin, INPUT_PULLUP);
            }
            if (!gpio) {
                gpio = GPIO::getGPIO(pin, INPUT_PULLDOWN);
            }
            printf("GPIO[%" PRIu32 "] intr, val: %d\n", gpio->getPin(), gpio->read());
        }
    }
}
*/

std::shared_ptr<GPIO> GPIO::getGPIO(gpio_num_t gpio, gpio_pinmode_t pin_mode /* , gpio_intr_mode_t intr */) {
    if (gpio < GPIO_NUM_0 || gpio >= GPIO_NUM_MAX) {
        ESP_LOGE("GPIO", "Invalid gpio pin provided");
        return nullptr;
    }
    if (m_instances[gpio]) {
        gpio_num_t i_gpio_pin = m_instances[gpio]->getPin();
        gpio_pinmode_t i_gpio_pin_mode = m_instances[gpio]->getPinMode();

        if (i_gpio_pin == GPIO_NUM_NC) {
            return nullptr;
        } else if (i_gpio_pin == gpio && i_gpio_pin_mode == pin_mode) {
            return m_instances[gpio];
        }
        return nullptr;
    } else {
        m_instances[gpio] = std::make_shared<GPIO>(GPIO(gpio, pin_mode /* , intr */));
        return m_instances[gpio];
    }
}

GPIO::GPIO(gpio_num_t pin, gpio_pinmode_t pin_mode /* , gpio_intr_mode_t intr */) : m_pin(pin), m_pin_mode(pin_mode) {
    gpio_config_t conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    conf.mode = (gpio_mode_t)(pin_mode & (INPUT | OUTPUT));
    if (pin_mode & OPEN_DRAIN) {
        conf.mode = (gpio_mode_t)(conf.mode | GPIO_MODE_DEF_OD);
    }
    if (pin_mode & PULLUP) {
        conf.pull_up_en = GPIO_PULLUP_ENABLE;
    }
    if (pin_mode & PULLDOWN) {
        conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    }
    if (gpio_config(&conf) != ESP_OK) {
        ESP_LOGE("GPIO", "pin %" PRIu32 " failed to configure", (uint32_t)pin);
        return;
    }
    /*
        if (intr != INTR_DISABLE) {
            gpio_set_intr_type((gpio_num_t)pin, GPIO_INTR_ANYEDGE);
            if (isr_running == 0) {
                xTaskCreate(gpio_isr_task, "gpio_isr_task", 2048, NULL, 10, NULL);
                gpio_install_isr_service(0);
                isr_running = 1;
            }
            gpio_isr_handler_add((gpio_num_t)pin, gpio_isr_handler, (void *)pin);
        } */
}

gpio_state_t GPIO::read() {
    return gpio_get_level((gpio_num_t)m_pin) ? HIGH : LOW;
}

bool GPIO::write(gpio_state_t value) {
    return (gpio_set_level((gpio_num_t)m_pin, value) == ESP_OK) ? true : false;
}

double GPIO::pulseIn(gpio_state_t state, unsigned long timeout) {
    rtc_cpu_freq_config_t conf;
    uint32_t start_cycle_count, pulse_start_cycle_count, max_timeout, timeout_cycles, freq_mhz;

    rtc_clk_cpu_freq_get_config(&conf);
    freq_mhz = conf.freq_mhz;

    max_timeout = UINT_MAX / freq_mhz;
    if (timeout > max_timeout) {
        timeout = max_timeout;
    }
    timeout_cycles = timeout * freq_mhz;
    start_cycle_count = esp_cpu_get_cycle_count();
    WAIT_FOR_PIN_STATE(!state);
    WAIT_FOR_PIN_STATE(state);
    pulse_start_cycle_count = esp_cpu_get_cycle_count();
    WAIT_FOR_PIN_STATE(!state);

    return ((esp_cpu_get_cycle_count() - pulse_start_cycle_count) / freq_mhz);
}

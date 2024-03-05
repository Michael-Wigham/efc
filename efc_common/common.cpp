/**
 * @file common.cpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 */
#include <esp_timer.h>

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

/**
 * @file pwm_controller.cpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @brief //TODO
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <esp_log.h>

#include <tuple>

#include <efc_drivers/pwm_controller.hpp>

PWMController::PWMController(uint32_t timebase_resolution, uint32_t timebase_period) {
    // Timer and operator config
    mcpwm_timer_config_t timer_config = {
        .group_id = m_global_group_id,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = timebase_resolution,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
        .period_ticks = timebase_period};

    mcpwm_operator_config_t operator_config = {
        .group_id = m_global_group_id
    };

    // Create and connect timer and operator
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &m_pwm_timer));
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &m_pwm_operator));
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(m_pwm_operator, m_pwm_timer));
    ESP_ERROR_CHECK(mcpwm_timer_enable(m_pwm_timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(m_pwm_timer, MCPWM_TIMER_START_NO_STOP));
}

std::pair<mcpwm_cmpr_handle_t, mcpwm_gen_handle_t> PWMController::create_config_for_gpio(int gpio) {
    // Comparator and generator config
    mcpwm_comparator_config_t comparator_config = {
        .flags = {
            .update_cmp_on_tez = true}};

    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = gpio
    };

    // Create comparator and generator
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t generator;
    ESP_ERROR_CHECK(mcpwm_new_comparator(m_pwm_operator, &comparator_config, &comparator));
    ESP_ERROR_CHECK(mcpwm_new_generator(m_pwm_operator, &generator_config, &generator));

    // Configure comparator events and return config pair
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator, MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator, MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    return {comparator, generator};
}

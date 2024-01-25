/**
 * @file pwm_controller.hpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @brief //TODO
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EFC_PWM_CONTROLLER_HPP
#define EFC_PWM_CONTROLLER_HPP

#include <driver/mcpwm_prelude.h>

#include <tuple>

class PWMController {
public:
    /**
     * @brief Construct a new Pwm Controller object
     *
     * @param timebase_resolution
     * @param timebase_period
     */
    PWMController(uint32_t timebase_resolution = 1000000, uint32_t timebase_period = 20000);

    /**
     * @brief Create a config for gpio object
     *
     * @param gpio
     * @return std::pair<mcpwm_cmpr_handle_t, mcpwm_gen_handle_t>
     */
    std::pair<mcpwm_cmpr_handle_t, mcpwm_gen_handle_t> create_config_for_gpio(int gpio);

private:
    // Static config
    inline static int m_global_group_id = 0;

    // Instance state
    mcpwm_timer_handle_t m_pwm_timer;
    mcpwm_oper_handle_t m_pwm_operator;
};

#endif // ifndef EFC_PWM_CONTROLLER_HPP

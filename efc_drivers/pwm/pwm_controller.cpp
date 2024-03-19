#include <efc_drivers/pwm_controller.hpp>

PWMController *PWMController::get_pwm_controller(gpio_num_t gpio) {
    for (int i = 0; i < LEDC_CHANNEL_MAX; i++) {
        if (m_instances[i]) {
            if (m_instances[i]->m_gpio == gpio) {
                return m_instances[i];
            }
        }
    }
    for (int i = 0; i < LEDC_CHANNEL_MAX; i++) {
        if (m_instances[i] == nullptr) {
            m_instances[i] = new PWMController(gpio, static_cast<ledc_channel_t>(i));
            return m_instances[i];
        }
    }
    return nullptr;
}

PWMController::PWMController(gpio_num_t gpio, ledc_channel_t channel) : m_gpio(gpio), m_channel(channel) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK};

    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_ch = {
        .gpio_num = gpio,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
        .flags = {
            .output_invert = 0}};

    ledc_channel_config(&ledc_ch);
}

void PWMController::set_duty(uint32_t duty) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel);
}

uint32_t PWMController::get_duty() {
    return ledc_get_duty(LEDC_LOW_SPEED_MODE, m_channel);
}

#include "servo.h"

uint32_t angel_rotation_to_duty(uint32_t const t_on, uint32_t const t_off, uint32_t const duty) {
    return (uint32_t) (((double)t_on / (t_on + t_off)) * duty);
}

esp_err_t setup_servo_ledc_timer(uint32_t const freq_hz, ledc_timer_t const timer_num, ledc_timer_bit_t const timer_bit) {
    ledc_timer_config_t const timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false,
        .freq_hz = freq_hz,
        .timer_num = timer_num,
        .duty_resolution = timer_bit
    };

    return ledc_timer_config(&timer);
}

esp_err_t setup_servo_ledc_channel(gpio_num_t const gpio_num, ledc_channel_t const channel, ledc_timer_t const timer, uint32_t const duty, int const point) {
    ledc_channel_config_t const config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = gpio_num,
        .channel = channel,
        .timer_sel = timer,
        .duty = duty,
        .hpoint = point,
        .flags = { .output_invert = SERVO_INVERT_DEF_CHANNEL }
    };

    return ledc_channel_config(&config);
}
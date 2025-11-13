#include "motor.h"

esp_err_t setup_motor_direction_pins() {
    gpio_config_t const config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << CONFIG_GPIO_BACK_LEFT_MOV_FWD) |(1ULL << CONFIG_GPIO_BACK_LEFT_MOV_BACK) | (1ULL << CONFIG_GPIO_BACK_RIGHT_MOV_FWD) |(1ULL << CONFIG_GPIO_BACK_RIGHT_MOV_BACK),
        .intr_type = GPIO_INTR_ANYEDGE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    return gpio_config(&config);
}

esp_err_t setup_motor_ledc_timer( ledc_timer_t const timer_num, ledc_timer_bit_t const timer_bit ) {
    ledc_timer_config_t const config = {
        .timer_num = timer_num,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false,
        .duty_resolution = timer_bit,
        .freq_hz = 5000,
    };

    return ledc_timer_config(&config);
}

esp_err_t setup_motor_ledc_channel( gpio_num_t const gpio_num, ledc_channel_t const channel, ledc_timer_t const timer ) {
    ledc_channel_config_t const config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = gpio_num,
        .channel = channel,
        .timer_sel = timer,
        .duty = 0,
        .hpoint = 0,
        .flags = { .output_invert = MOTOR_INVERT_DEF_CHANNEL },
    };

    return ledc_channel_config(&config);
}
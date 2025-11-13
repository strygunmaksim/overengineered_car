#ifndef SERVO_H_
#define SERVO_H_

/** TODO! REFACTOR */
#include "driver/ledc.h"

#define SERVO_INVERT_DEF_CHANNEL 0

esp_err_t setup_servo_ledc_timer(uint32_t const freq_hz, ledc_timer_t const timer_num, ledc_timer_bit_t const timer_bit);
esp_err_t setup_servo_ledc_channel(gpio_num_t const gpio_num, ledc_channel_t const channel, ledc_timer_t const timer, uint32_t const duty, int const point);

#endif
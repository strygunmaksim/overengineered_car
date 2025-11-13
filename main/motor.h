#ifndef MOTOR_H_
#define MOTOR_H_

/** TODO! REFACTOR */
#include "driver/gpio.h"
#include "driver/ledc.h"

/** TODO! move gpio to config and add 4WD */
#define CONFIG_GPIO_BACK_LEFT_MOV_FWD  2
#define CONFIG_GPIO_BACK_LEFT_MOV_BACK 4
#define CONFIG_GPIO_BACK_LEFT_IN_BACK  15

#define CONFIG_GPIO_BACK_RIGHT_MOV_FWD  16
#define CONFIG_GPIO_BACK_RIGHT_MOV_BACK 17
#define CONFIG_GPIO_BACK_RIGHT_IN_BACK  5
/** ------------------------------------  */

#define MOTOR_INVERT_DEF_CHANNEL 0
#define TAG_MOTOR "MOTOR"

esp_err_t setup_motor_direction_pins();
esp_err_t setup_motor_ledc_timer(ledc_timer_t const timer_num, ledc_timer_bit_t const timer_bit);
esp_err_t setup_motor_ledc_channel( gpio_num_t const gpio_num, ledc_channel_t const channel, ledc_timer_t const timer );

#endif
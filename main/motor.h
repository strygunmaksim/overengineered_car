#ifndef MOTOR_H_
#define MOTOR_H_


#include "driver/gpio.h"
#include "driver/ledc.h"

#define GPIO_MOTOR_BL_FWD                       GPIO_NUM_32
#define GPIO_MOTOR_BL_BACK                      GPIO_NUM_33
#define GPIO_MOTOR_BR_FWD                       GPIO_NUM_26
#define GPIO_MOTOR_BR_BACK                      GPIO_NUM_25

/** It will be necessary assign (forward moving) */
#define GPIO_MOTOR_FL_FWD                       GPIO_NUM_32
#define GPIO_MOTOR_FL_BACK                      GPIO_NUM_33
#define GPIO_MOTOR_FR_FWD                       GPIO_NUM_26
#define GPIO_MOTOR_FR_BACK                      GPIO_NUM_25

#define GPIO_ENA_BL                             GPIO_NUM_27
#define GPIO_ENB_BR                             GPIO_NUM_14
/** It will be necessary assign (forward moving) */
#define GPIO_ENA_FL                             GPIO_NUM_27
#define GPIO_ENB_FR                             GPIO_NUM_14

#define NUM_CHANNEL_BL_FWD                      LEDC_CHANNEL_0
#define NUM_CHANNEL_BR_FWD                      LEDC_CHANNEL_1
/** It will be necessary assign (forward moving) */
#define NUM_CHANNEL_FL_FWD                      LEDC_CHANNEL_2
#define NUM_CHANNEL_FR_FWD                      LEDC_CHANNEL_3

#define MOTOR_DC_TT_HZ                          1000
#define MOTOR_TIMER_NUM                         LEDC_TIMER_0

#define DUTY(DUTY_RESOLUTION)                   ((1 << DUTY_RESOLUTION) - 1)

void vMSetupTimerForMotors();
void vMSetupGPIOMotors();

void vMStartMotor(gpio_num_t const gpio);
void vMStopMotor(gpio_num_t const gpio);
void vMMovingForward(gpio_num_t const gpio);
void vMMovingBack(gpio_num_t const gpio);

void vMSetupChannel(gpio_num_t const gpio, ledc_channel_t const channel, uint32_t const duty);

/** TEST */
void change_duty();

#endif
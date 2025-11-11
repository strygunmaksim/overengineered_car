#ifndef MOTOR_H_
#define MOTOR_H_

#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_log.h"


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

#define FLAG_ENGINE_STAR                        (1 << 0)
#define FLAG_ENGINE_BLOCK                       (1 << 1)

#define HIGH_LEVEL                              1
#define LOW_LEVEL                               0

#define GPIO_INDICATOR_OPERATION_INDICATOR      GPIO_NUM_4
#define GPIO_BUTTON_ENGINE_CONTROL_START_STOP   GPIO_NUM_5

typedef struct {
    unsigned int status_engine: 1;
} engine_flag;

void vMSetupGPIOMotors();
void vMSetupTimerForMotors();

void vMStartMotor(gpio_num_t const gpio);
void vMStopMotor(gpio_num_t const gpio);
void vMMovingForward(gpio_num_t const gpio);
void vMMovingBack(gpio_num_t const gpio);

void vMEngineStartEngine();
void VMEngineStopEngine();

void vMSetupChannel(gpio_num_t const gpio, ledc_channel_t const channel, uint32_t const duty);

/** TEST */
void change_duty();

#endif
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define GPIO_MOTOR_BL_FWD                       GPIO_NUM_32
#define GPIO_MOTOR_BL_BACK                      GPIO_NUM_33

#define GPIO_MOTOR_BR_FWD                       GPIO_NUM_26
#define GPIO_MOTOR_BR_BACK                      GPIO_NUM_25

#define GPIO_ENA_BL                             GPIO_NUM_27
#define GPIO_ENB_BR                             GPIO_NUM_14

#define HIGH_LEVEL                              1
#define LOW_LEVEL                               0

#define MOTOR_DC_TT_HZ                          1000
#define MOTOR_TIMER_NUM                         LEDC_TIMER_0
#define DUTY_RESOLUTION_BIT                     LEDC_TIMER_8_BIT
#define DUTY(DUTY_RESOLUTION)                   ((1U << (DUTY_RESOLUTION)) - 1)
#define DUTY_PERCENT(DUTY_RESOLUTION, percent)  ((DUTY(DUTY_RESOLUTION) * (percent)) / (100))

#define STACK_SIZE_TASK_MT_CONTROL              2048

static const char *TAG = "main";

void vSetupGPIOOutputPin(unsigned long long const bit_mask)
{
    gpio_config_t config = {};

    config.mode         = GPIO_MODE_OUTPUT;
    config.intr_type    = GPIO_INTR_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en   = GPIO_PULLUP_DISABLE;
    config.pin_bit_mask = bit_mask;

    gpio_config(&config);
}

void vSetupMotorLEDCTimer()
{
    ledc_timer_config_t config = {};

    config.speed_mode = LEDC_LOW_SPEED_MODE;
    config.duty_resolution = DUTY_RESOLUTION_BIT;
    config.timer_num = MOTOR_TIMER_NUM;
    config.freq_hz = MOTOR_DC_TT_HZ;
    config.clk_cfg = LEDC_AUTO_CLK;
    config.deconfigure = false;

    ledc_timer_config(&config);
}

void vSetupLEDCMotorChannel(int const gpio_num, ledc_channel_t const channel, ledc_timer_t const timer)
{
    ledc_channel_config_t config = {};

    config.gpio_num = gpio_num;
    config.speed_mode = LEDC_LOW_SPEED_MODE;
    config.channel = channel;
    config.intr_type = LEDC_INTR_DISABLE;
    config.timer_sel = timer;
    config.duty      = 0;
    config.hpoint    = 0;

    ledc_channel_config(&config);
}

void app_main(void) {
    /** Setup LEDC time for motor WD */
    vSetupMotorLEDCTimer();
    /** Setup LEDC motor channel BACK LEFT */
    vSetupLEDCMotorChannel(GPIO_MOTOR_BL_FWD, LEDC_CHANNEL_0, MOTOR_TIMER_NUM);
    vSetupLEDCMotorChannel(GPIO_MOTOR_BL_BACK, LEDC_CHANNEL_1, MOTOR_TIMER_NUM);
    /** Setup LEDC motor channel BACK RIGHT */
    vSetupLEDCMotorChannel(GPIO_MOTOR_BR_FWD, LEDC_CHANNEL_2, MOTOR_TIMER_NUM);
    vSetupLEDCMotorChannel(GPIO_MOTOR_BR_BACK, LEDC_CHANNEL_3, MOTOR_TIMER_NUM);

    /** Setup start engine */
    vSetupGPIOOutputPin((1ULL << GPIO_ENA_BL) | (1ULL << GPIO_ENB_BR));

    for ( ;; )
    {
        vTaskDelay( pdMS_TO_TICKS(100) );
    }
}




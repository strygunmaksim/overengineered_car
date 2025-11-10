#include "freertos/projdefs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "motor.h"

static const char *TAG = "motor";

void vMSetupTimerForMotors() {
    ledc_timer_config_t const config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = MOTOR_DC_TT_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };

    ledc_timer_config(&config);
}

void vMSetupGPIOMotors() {
    gpio_config_t const config = {
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << GPIO_ENA_BL) | (1ULL << GPIO_ENB_BR) | (1ULL << GPIO_MOTOR_BL_FWD) | (1ULL << GPIO_MOTOR_BR_FWD),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    gpio_config(&config);
}

void vMStartMotor(gpio_num_t const gpio) {
    gpio_set_level(gpio, 1);
}

void vMStopMotor(gpio_num_t const gpio) {
    gpio_set_level(gpio, 0);
}

void vMovingForward(gpio_num_t const gpio) {
    gpio_set_level(gpio, 1);
}

void vMMovingBack(gpio_num_t const gpio) {
    gpio_set_level(gpio, 1);
}

void vMSetupChannel(gpio_num_t const gpio, ledc_channel_t const channel, uint32_t const duty) {
    ledc_channel_config_t const config = {
        .gpio_num = gpio,
        .channel = channel,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = duty,
        .hpoint = 0
    };

    ledc_channel_config(&config);
}

/** TEST */
void change_duty() {
    vMStartMotor(GPIO_MOTOR_BL_FWD);
    vMStartMotor(GPIO_MOTOR_BR_FWD);
    vMovingForward(GPIO_MOTOR_BL_FWD);
    vMovingForward(GPIO_MOTOR_BR_FWD);

    for (int i = 0; i < DUTY(LEDC_TIMER_8_BIT); i += 20) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, NUM_CHANNEL_BL_FWD, i);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, NUM_CHANNEL_BR_FWD, i);

        ledc_update_duty(LEDC_LOW_SPEED_MODE, NUM_CHANNEL_BL_FWD);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, NUM_CHANNEL_BR_FWD);

        ESP_LOGI(TAG, "%d", i);

        vTaskDelay( pdMS_TO_TICKS( 5000 ) );
    }

    vMStopMotor(GPIO_MOTOR_BL_FWD);
}
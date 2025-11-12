#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver//ledc.h"

// #include "motor.h"

static const char *TAG = "MAIN";

// void IRAM_ATTR vMStarStopControl(void *argv) {
//     engine_flag *config = (argv);
//
//     /** see analizator create other task */
//     if ( gpio_get_level(config->status_engine) ) {
//         vMEngineStartEngine();
//     } else {
//         VMEngineStopEngine();
//     }
// }
//
// void vMStarStopControl3(void *argv) {
//     engine_flag *config = (argv);
//
//     /** see analizator create other task */
//     if ( config->status_engine ) {
//         ESP_LOGI(TAG, "CON");
//
//         VMEngineStopEngine();
//     } else {
//         ESP_LOGI(TAG, "CON 2");
//         vMEngineStartEngine();
//     }
// }

int percent_to_duty(short const percent, ledc_timer_bit_t const duty_bit) {
    int duty_num = (1 << duty_bit) - 1;

    if (percent >= 100) {
        return duty_num;
    }

    if ( percent < 0 ) {
        return 0;
    }

    return (duty_num * percent) / 100;
}

int angle_to_duty(uint8_t angle) {
    /** !TODO REFACTOR */
    int const pulse = 500 + (2500 - 500) * angle / 180;

   return (pulse * ((1 << LEDC_TIMER_8_BIT) - 1)) / 20000;
}

void task1(void *argv) {
    int const duty = (1 << LEDC_TIMER_8_BIT) - 1;

    for (int i = 0; i <= 180; i += 1) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, angle_to_duty(i));
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        ESP_LOGI(TAG, "MOVE %d-180", i);
        vTaskDelay( pdMS_TO_TICKS(20) );
    }

    for (int i = duty; i >= 0; i--) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, angle_to_duty(i));
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        ESP_LOGI(TAG, "MOVE 180-%d", i);
        vTaskDelay( pdMS_TO_TICKS(20) );
    }

    for ( ;; ) {
        vTaskDelay( pdMS_TO_TICKS(1000) );
    }
}

void app_main(void) {
    // engine_flag engine_status = {.status_engine = 0 };
    //
    // /** !TODO error hangling */
    // // vMSetupGPIOMotors();
    // /** LEDC for control */
    // // vMSetupTimerForMotors();
    // // /** !TODO add full 4WD */
    // // vMSetupChannel(GPIO_ENA_BL, LEDC_CHANNEL_0, MOTOR_TIMER_NUM);
    // // vMSetupChannel(GPIO_ENB_BR, LEDC_CHANNEL_1, MOTOR_TIMER_NUM);
    //

    gpio_config_t config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << GPIO_NUM_2),
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    gpio_config(&config);
    gpio_set_level(GPIO_NUM_2, 1);

    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };
    ledc_timer_config(&timer_config);

    ledc_channel_config_t channel_config = {
        .gpio_num = GPIO_NUM_15,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .flags = { .output_invert = 0 },
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&channel_config);

    xTaskCreate(task1, "task1", 2048, NULL, 2, NULL);

    for ( ;; ) {
        vTaskDelay( pdMS_TO_TICKS(100) );
    }
}

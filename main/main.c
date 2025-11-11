#include "esp_attr.h"

#include "motor.h"

static const char *TAG = "MAIN";

void IRAM_ATTR vMStarStopControl(void *argv) {
    engine_flag *config = (argv);

    /** see analizator create other task */
    if ( gpio_get_level(config->status_engine) ) {
        vMEngineStartEngine();
    } else {
        VMEngineStopEngine();
    }
}

void vMStarStopControl3(void *argv) {
    engine_flag *config = (argv);

    /** see analizator create other task */
    if ( config->status_engine ) {
        ESP_LOGI(TAG, "CON");

        VMEngineStopEngine();
    } else {
        ESP_LOGI(TAG, "CON 2");
        vMEngineStartEngine();
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
    // unsigned long long bit_mask =
    //         (1ULL << GPIO_ENA_BL) |
    //         (1ULL << GPIO_ENB_BR) |
    //         (1ULL << GPIO_MOTOR_BL_FWD) |
    //         (1ULL << GPIO_MOTOR_BL_BACK) |
    //         (1ULL << GPIO_MOTOR_BR_FWD ) |
    //         (1ULL << GPIO_MOTOR_BR_BACK);
    //
    // gpio_config_t const config = {
    //     .mode = GPIO_MODE_OUTPUT,
    //     .intr_type = GPIO_INTR_DISABLE,
    //     .pin_bit_mask =  bit_mask,
    //     .pull_down_en = GPIO_PULLDOWN_DISABLE,
    //     .pull_up_en = GPIO_PULLUP_DISABLE,
    // };
    //
    // gpio_config(&config);
    //
    // gpio_set_level(GPIO_ENA_BL, 1);
    // gpio_set_level(GPIO_ENB_BR, 1);
    //
    // gpio_set_level(GPIO_MOTOR_BL_FWD, 1);
    // gpio_set_level(GPIO_MOTOR_BL_BACK, 0);
    // // vMStartMotor(GPIO_MOTOR_BL_FWD);
    // gpio_set_level(GPIO_MOTOR_BR_FWD, 1);
    // gpio_set_level(GPIO_MOTOR_BR_BACK, 0);
    //
    //
    // // vMStarStopControl3((void *) &engine_status);
    //
    //
    // // vMStartMotor(GPIO_MOTOR_BR_FWD);
    //
    // // gpio_isr_handler_add(GPIO_BUTTON_ENGINE_CONTROL_START_STOP, vMStarStopControl, (void *) &engine_status);
    //
    // /** RUN TEST */
    // // xTaskCreate(change_duty, "Change Duty", 2048, NULL, 5, NULL);
    //
    // for ( ;; )
    // {
    //     vTaskDelay( pdMS_TO_TICKS(100) );
    // }

    unsigned long long bit_mask =
          (1ULL << GPIO_ENA_BL) |
          (1ULL << GPIO_ENB_BR) |
          (1ULL << GPIO_MOTOR_BL_FWD) |
          (1ULL << GPIO_MOTOR_BL_BACK) |
          (1ULL << GPIO_MOTOR_BR_FWD ) |
          (1ULL << GPIO_MOTOR_BR_BACK);

    gpio_config_t const config = {
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = bit_mask,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    gpio_config(&config);

    // включаем драйвер
    gpio_set_level(GPIO_ENA_BL, 1);
    gpio_set_level(GPIO_ENB_BR, 1);

    // левый мотор вперед
    gpio_set_level(GPIO_MOTOR_BL_FWD, 1);
    gpio_set_level(GPIO_MOTOR_BL_BACK, 0);

    // правый мотор вперед
    gpio_set_level(GPIO_MOTOR_BR_FWD, 1);
    gpio_set_level(GPIO_MOTOR_BR_BACK, 0);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

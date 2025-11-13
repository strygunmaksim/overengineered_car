#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "motor.h"

/** TODO! move gpio to config and add 4WD */
#define CONFIG_TIMER_SERVO_NUM     LEDC_TIMER_0
#define CONFIG_TIME_MOTOR_NUM      LEDC_TIMER_1

#define CONFIG_TIME_BIT_MOTOR_NUM  LEDC_TIMER_1_BIT
#define CONFIG_BACK_LEFT_IN_NUM    LEDC_CHANNEL_1
#define CONFIG_BACK_RIGHT_IN_NUM   LEDC_CHANNEL_2


/** -----------------------------------  */

#define LOG_ERR(func) do { \
    esp_err_t err = (func); \
    if ( err != ESP_OK ) { \
        ESP_LOGE("RESTART ERR", "%s filed %s", #func, esp_err_to_name(err)); \
    } \
} while (0)


/** JUST TEST */
void task1( void *pvParams ) {
    uint32_t duty = 1U << *( (uint32_t *) pvParams);

    for ( ;; ) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, CONFIG_BACK_LEFT_IN_NUM, duty);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, CONFIG_BACK_RIGHT_IN_NUM, duty);

        ledc_update_duty(LEDC_LOW_SPEED_MODE, CONFIG_BACK_LEFT_IN_NUM);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, CONFIG_BACK_RIGHT_IN_NUM);

        vTaskDelay( pdMS_TO_TICKS(100) );
    }
}

void app_main(void) {
    LOG_ERR(setup_motor_direction_pins());
    LOG_ERR(setup_motor_ledc_timer(CONFIG_TIME_MOTOR_NUM, CONFIG_TIME_BIT_MOTOR_NUM));

    LOG_ERR(setup_motor_ledc_channel(CONFIG_GPIO_BACK_LEFT_IN_BACK, CONFIG_BACK_LEFT_IN_NUM, CONFIG_TIME_MOTOR_NUM));
    LOG_ERR(setup_motor_ledc_channel(CONFIG_GPIO_BACK_RIGHT_IN_BACK, CONFIG_BACK_RIGHT_IN_NUM, CONFIG_TIME_MOTOR_NUM));

    xTaskCreate(task1, "task1", 2048, (void *) CONFIG_TIME_BIT_MOTOR_NUM, 1, NULL);

    for ( ;; ) {
        vTaskDelay( pdMS_TO_TICKS(100) );
    }
}

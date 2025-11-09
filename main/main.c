#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"

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

#define STACK_SIZE_TASK_JOYSTICK_MOVE           2048
#define DED_ZONE_JOYSTICK_MOVE                  500

static const char *TAG = "main";
static const char *TAG_JOYSTICK_MOVE_DC = "Joystick";

void vMSetupGPIOOutputPin(unsigned long long const bit_mask) {
    const gpio_config_t config = {
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pin_bit_mask = bit_mask
    };

    gpio_config(&config);
}

void vMSetupMotorLEDCTimer() {
    const ledc_timer_config_t config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = DUTY_RESOLUTION_BIT,
        .timer_num = MOTOR_TIMER_NUM,
        .freq_hz = MOTOR_DC_TT_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false,
    };

    ledc_timer_config(&config);
}

void vMSetupLEDCMotorChannel(int const gpio_num, ledc_channel_t const channel, ledc_timer_t const timer) {
    const ledc_channel_config_t config = {
        .gpio_num = gpio_num,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = timer,
        .duty = 0,
        .hpoint = 0,
    };

    ledc_channel_config(&config);
}

void vReadAdcSmooth(adc_oneshot_unit_handle_t const handle, adc_channel_t const channel, int *pValue)
{
    int count = 0;
    int value = 0;

    for ( int i = 0; i < 10; i++ )
    {
        adc_oneshot_read(handle, channel, &value);
        count += value;
    }

    *pValue = (count / 10);
}


void vMTaskJoystickMove(void *pvParameters) {
    const adc_oneshot_unit_init_cfg_t config_adc = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    adc_oneshot_unit_handle_t config_handler;
    // ESP_ERROR_CHECK !check
    adc_oneshot_new_unit(&config_adc, &config_handler);

    const adc_oneshot_chan_cfg_t config_channel = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };

    // ESP_ERROR_CHECK !check
    adc_oneshot_config_channel(config_handler, ADC_CHANNEL_6, &config_channel);
    adc_oneshot_config_channel(config_handler, ADC_CHANNEL_7, &config_channel);

    int x = 0;
    int y = 0;

    char level_dc = 0;

    for ( ;; )
    {
        vReadAdcSmooth(config_handler, ADC_CHANNEL_6, &y);
        vReadAdcSmooth(config_handler, ADC_CHANNEL_7, &x);

        ESP_LOGI(TAG_JOYSTICK_MOVE_DC, "X - %d, Y - %d", x, y);

        level_dc = gpio_get_level(GPIO_ENA_BL);

        if ( x > 2048 + DED_ZONE_JOYSTICK_MOVE && !level_dc)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, DUTY_PERCENT(DUTY_RESOLUTION_BIT, 30));
            // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, DUTY_PERCENT(DUTY_RESOLUTION_BIT, 30));
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, DUTY_PERCENT(DUTY_RESOLUTION_BIT, 30));
            // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, DUTY_PERCENT(DUTY_RESOLUTION_BIT, 30));
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);

            gpio_set_level(GPIO_ENA_BL, HIGH_LEVEL);
            gpio_set_level(GPIO_ENB_BR, HIGH_LEVEL);

            ESP_LOGI(TAG_JOYSTICK_MOVE_DC, "RUN MOTORS DC");
        }
        else if ( x < 2048 + DED_ZONE_JOYSTICK_MOVE && level_dc)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, DUTY_PERCENT(DUTY_RESOLUTION_BIT, 0));
            // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, DUTY_PERCENT(DUTY_RESOLUTION_BIT, 0));
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, DUTY_PERCENT(DUTY_RESOLUTION_BIT, 0));
            // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, DUTY_PERCENT(DUTY_RESOLUTION_BIT, 0));
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);


            gpio_set_level(GPIO_ENA_BL, LOW_LEVEL);
            gpio_set_level(GPIO_ENB_BR, LOW_LEVEL);

            ESP_LOGI(TAG_JOYSTICK_MOVE_DC, "STOP MOTORS DC");
        }

        vTaskDelay( pdMS_TO_TICKS(50) );
    }
}


void app_main(void) {
    /** Setup LEDC time for motor WD */
    vMSetupMotorLEDCTimer();
    /** Setup LEDC motor channel BACK LEFT */
    vMSetupLEDCMotorChannel(GPIO_MOTOR_BL_FWD, LEDC_CHANNEL_0, MOTOR_TIMER_NUM);
    vMSetupLEDCMotorChannel(GPIO_MOTOR_BL_BACK, LEDC_CHANNEL_1, MOTOR_TIMER_NUM);
    /** Setup LEDC motor channel BACK RIGHT */
    vMSetupLEDCMotorChannel(GPIO_MOTOR_BR_FWD, LEDC_CHANNEL_2, MOTOR_TIMER_NUM);
    vMSetupLEDCMotorChannel(GPIO_MOTOR_BR_BACK, LEDC_CHANNEL_3, MOTOR_TIMER_NUM);

    /** Setup start engine */
    vMSetupGPIOOutputPin((1ULL << GPIO_ENA_BL) | (1ULL << GPIO_ENB_BR));

    /** Check on error */
    xTaskCreate(
        vMTaskJoystickMove,
        "vMTaskJoystickMove",
        STACK_SIZE_TASK_JOYSTICK_MOVE,
        NULL,
        5,
        NULL
    );

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

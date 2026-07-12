#include <hal/ledc_types.h>
#include <soc/clk_tree_defs.h>
#include <math.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <freertos/idf_additions.h>
#include <esp_log.h>

void app_main(void)
{
    ledc_timer_config_t tc = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&tc);

    ledc_channel_config_t cc = {
        .gpio_num = GPIO_NUM_23,
        .speed_mode = tc.speed_mode,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = tc.timer_num,

        .duty = 0,
        .hpoint = 0
        // .sleep_mode = 
    };

    ledc_channel_config(&cc);

    int total_duty = pow(2, tc.duty_resolution); // 256, in case of 8
    while (true) {
        for (int duty = 0; duty < total_duty; duty++) {
            ledc_set_duty(tc.speed_mode, cc.channel, duty);
            ledc_update_duty(tc.speed_mode, cc.channel);
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        vTaskDelay(pdMS_TO_TICKS(2000));

        for (int duty = total_duty - 1; duty >= 0; --duty) {
            ledc_set_duty(tc.speed_mode, cc.channel, duty);
            ledc_update_duty(tc.speed_mode, cc.channel);
            vTaskDelay(pdMS_TO_TICKS(10));
        }

    }
}

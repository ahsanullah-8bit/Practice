#include "soc/gpio_num.h"
#include <driver/gpio.h>
#include <freertos/idf_additions.h>
#include <hal/gpio_types.h>
#include <portmacro.h>
#include <esp_log.h>
#include <stdint.h>
#include <sys/types.h>

void app_main(void)
{
    const char *TAG = "BlinkWithButton";
    const gpio_num_t GPIO_BUTTON = GPIO_NUM_22;
    const gpio_num_t GPIO_LED = GPIO_NUM_23;

    gpio_reset_pin(GPIO_BUTTON);
    gpio_reset_pin(GPIO_LED);
    gpio_set_direction(GPIO_BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_BUTTON, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);
    
    uint delay = 100; // ms
    uint msecs = 0;
    uint8_t total_levels = 0;
    uint8_t led_level = 0;

    while(true) {
        int level = gpio_get_level(GPIO_BUTTON);
        if (level)
            total_levels++;

        if ((total_levels == 2 && msecs >= 200 && msecs <= 900) || total_levels >= 10) {
            led_level = !led_level;
            gpio_set_level(GPIO_LED, led_level);
        }

        // if (level)
        //     ESP_LOGI(TAG, "LED Turned ON");
        // else
        //     ESP_LOGI(TAG, "LED Turned OFF");

        vTaskDelay(delay / portTICK_PERIOD_MS); // every 100ms
        msecs += delay;
        if (msecs >= 1000) {
            total_levels = 0;
            msecs = 0;
        }
    }
}

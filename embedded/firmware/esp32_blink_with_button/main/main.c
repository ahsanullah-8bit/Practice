#include <driver/gpio.h>
#include <freertos/idf_additions.h>
#include <hal/gpio_types.h>
#include <portmacro.h>
#include <esp_log.h>
#include <stdint.h>

void app_main(void)
{
    const char *TAG = "BlinkWithButton";
    const uint8_t GPIO_BUTTON = 22;
    const uint8_t GPIO_LED = 23;

    gpio_reset_pin(GPIO_BUTTON);
    gpio_reset_pin(GPIO_LED);
    gpio_set_direction(GPIO_BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_BUTTON, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);

    while(true) {
        int level = gpio_get_level(GPIO_BUTTON);
        gpio_set_level(GPIO_LED, level);

        if (level)
            ESP_LOGI(TAG, "LED Turned ON");
        else
            ESP_LOGI(TAG, "LED Turned OFF");

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

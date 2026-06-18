#include <stdint.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <hal/gpio_types.h>
#include <freertos/idf_additions.h>

typedef enum {
    OFF,
    ON
} state_t;

void app_main(void)
{
    const char *TAG = "BlinkLED";
    uint8_t GPIO_TO_BLINK = 5;
    state_t state = OFF;

    ESP_LOGI(TAG, "Configuring GPIO %d to blink LED.", GPIO_TO_BLINK);
    gpio_reset_pin(GPIO_TO_BLINK);
    gpio_set_direction(GPIO_TO_BLINK, GPIO_MODE_OUTPUT);

    while (true) {
        ESP_LOGI(TAG, "Turning LED %s", state == OFF ? "OFF" : "ON");
        gpio_set_level(GPIO_TO_BLINK, state);
        state = !state;

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#include <freertos/idf_additions.h>
#include <driver/gpio.h>
#include <hal/gpio_types.h>
#include <portmacro.h>
#include <stdint.h>
#include <esp_log.h>

typedef enum : uint32_t {
    STOP,
    READY,
    GO
} State;

void app_main(void)
{
    const char *TAG = "BlinkLED";
    const uint8_t GPIO_PIN23 = 23;
    const uint8_t GPIO_PIN22 = 22;
    const uint8_t GPIO_PIN21 = 21;
    gpio_reset_pin(GPIO_PIN23);
    gpio_reset_pin(GPIO_PIN22);
    gpio_reset_pin(GPIO_PIN21);
    gpio_set_direction(GPIO_PIN23, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_PIN22, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_PIN21, GPIO_MODE_OUTPUT);

    State state = STOP;
    while(true) {
        const char* state_str = state == STOP ? "Stopped"
                                    : state == READY ? "Ready" : "Flowing";
        ESP_LOGI(TAG, "Traffic %s", state_str);

        gpio_set_level(GPIO_PIN23, state == STOP);
        gpio_set_level(GPIO_PIN22, state == READY);
        gpio_set_level(GPIO_PIN21, state == GO);

        state = (state + 1) % 3;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/idf_additions.h>
#include <freertos/projdefs.h>
#include <hal/gpio_types.h>
#include <soc/clk_tree_defs.h>
#include <soc/gpio_num.h>
#include <driver/uart.h>
#include <driver/gpio.h>
#include <hal/uart_types.h>

void app_main(void)
{
    const char *TAG = "SCI";
    gpio_port_t GPIO_LED = GPIO_NUM_23;
    const int uart_buffer_size = 1024 * 2;
    uart_port_t up = UART_NUM_0;

    uart_config_t config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };
    ESP_ERROR_CHECK(uart_param_config(up, &config));
    ESP_ERROR_CHECK(uart_set_pin(up, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(up, uart_buffer_size, uart_buffer_size, 10, NULL, 0));

    gpio_reset_pin(GPIO_LED);
    gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);

    enum {
        OFF, ON, UKNOWN
    } status = OFF;

    uint8_t data[128];
    while (true) {
        size_t res = uart_read_bytes(up, data, sizeof(data) - 1, pdMS_TO_TICKS(100));
        
        if (res > 0) {
            data[res] = '\0';
            if (strcmp((char*)data, "led on") == 0) {
                status = ON;
                ESP_LOGI(TAG, "Turning LED ON");
            } else if (strcmp((char*)data, "led off") == 0) {
                status = OFF;
                ESP_LOGI(TAG, "Turning LED OFF");
            } else if (strcmp((char*)data, "status") == 0) {
                ESP_LOGI(TAG, "Status: %s", status == OFF ? "OFF" : "ON");
            } else
                status = UKNOWN;

            if (status != UKNOWN)
                gpio_set_level(GPIO_LED, status);
            else {
                ESP_LOGI(TAG, "Paste either of these commands:\n  1. led on\n  2. led off\n  3. status\n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    ESP_ERROR_CHECK(uart_driver_delete(up));
}

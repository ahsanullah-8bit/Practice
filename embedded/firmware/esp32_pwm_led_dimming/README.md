# PWD LED Dimming

Fades-in and out an LED, connected to GPIO23 of an ESP32 WROOM32, using LEDC module. Wokwi can't simulate it the same way, you just won't see the effect until it fades-out completely.

It takes 2550ms to fade-in, waits for 2s and fades-out again in 2550ms. Not exactly 2550ms, because we're using a task delay, it'll probably be taking more than that.

## Build
I've only built it following the official Espressif [ESP-IDF setup for VS Code](https://docs.espressif.com/projects/esp-idf/en/v6.0.1/esp32/get-started/index.html#get-started). Follow the setup and build with VS Code ESP-IDF extension.

It's not part of the root [CMakeLists.txt](../../../CMakeLists.txt) and must be built alone. It was tested on a real ESP32 WROOM32 as well as Wokwi.

## Wokwi
The diagram uses a `board-esp32-devkit-c-v4`, which has an identical setup, because the one I've (ESP32 WROOM32) is not available in Wokwi. Pin outs are similar to that of the hardcoded. You can also debug using wokwi by:

* Launching `Wokwi: Start Simulator and Wait for Debugger` from the command palette.
* Go to `Run and Debug` tab, select the `Wokwi GDB Debug`, then run.

## Notes

* **PWM**: Pulse Width Modulation or Pulse Duration Module **(PDM)** or Pulse Length Modulation **(PLM)**, mimics the analog signals using digital pulse, see [more](https://en.wikipedia.org/wiki/Pulse-width_modulation).

* **Duty Cycle**: Fraction of one period in which a signal or system is active. see [more](https://en.wikipedia.org/wiki/Duty_cycle).

* **[LEDC](https://docs.espressif.com/projects/esp-idf/en/v6.0.1/esp32/api-reference/peripherals/ledc.html)**: The official guide to LEDC.
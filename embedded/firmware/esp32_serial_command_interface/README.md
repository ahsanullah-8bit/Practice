# Serial Command Interface

Turns an LED ON and OFF connected to GPIO23 of an ESP32 WROOM32, using UART serial communication. There are 3 available commands:

* `led on`
* `led off`
* `status`

Note however, you can't type the command. It's too much work and still can't avoid the verbosity if I try to implement that. You
must copy and paste the command, otherwise only single characters will be sent to the ESP32.

## Build
I've only built it following the official Espressif [ESP-IDF setup for VS Code](https://docs.espressif.com/projects/esp-idf/en/v6.0.1/esp32/get-started/index.html#get-started). Follow the setup and build with VS Code ESP-IDF extension.

It's not part of the root [CMakeLists.txt](../../../CMakeLists.txt) and must be built alone. It was tested on a real ESP32 WROOM32 as well as Wokwi.

## Wokwi
Tested using a real board and still don't know how to work it with Wokwi. But the diagrams are available. It uses a `board-esp32-devkit-c-v4`, which has an identical setup, because the one I've (ESP32 WROOM32) is not available in Wokwi. Pin outs are similar to that of the hardcoded. You can also debug using wokwi by:

* Launching `Wokwi: Start Simulator and Wait for Debugger` from the command palette.
* Go to `Run and Debug` tab, select the `Wokwi GDB Debug`, then run.

## Notes

* [Finite State Machine](https://en.wikipedia.org/wiki/Finite-state_machine)
* [Official UART Guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/uart.html)
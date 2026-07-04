# Blink Multiple LEDs

This a traffic lights like project that uses 3 LEDs (Red, Yellow & Green), mimics basic state change of traffic lights in a loop. It uses ESP32 WROOM32's GPIO 21, 22, and 23 pins as follows:
    * Red LED's anode is connected to GPIO23 pin.
    * Yellow's anode is connected to GPIO22 pin.
    * Green's anode is connected to GPIO21 pin.

## Build
I've only built it following the official Espressif [ESP-IDF setup for VS Code](https://docs.espressif.com/projects/esp-idf/en/v6.0.1/esp32/get-started/index.html#get-started). Follow the setup and build with VS Code ESP-IDF extension.

It's not part of the root [CMakeLists.txt](../../../CMakeLists.txt) and must be built alone. It was tested on a real ESP32 WROOM32 as well as Wokwi.

## Wokwi
The diagram uses a `board-esp32-devkit-c-v4`, which has an identical setup, because the one I've (ESP32 WROOM32) is not available in Wokwi. Pin outs are similar to that of the hardcoded. You can also debug using wokwi by:

* Launching `Wokwi: Start Simulator and Wait for Debugger` from the command palette.
* Go to `Run and Debug` tab, select the `Wokwi GDB Debug`, then run.
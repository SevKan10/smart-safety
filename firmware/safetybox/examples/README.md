# Peripheral test sketches

Each `.cpp` file in this directory is a standalone Arduino sketch for the
ESP32-S3. To build or upload one test, copy that file to `src/main.cpp`
(replace the existing file), then run PlatformIO.

| File | Peripheral | ESP32-S3 pins |
|---|---|---|
| `test_gps.cpp` | ATGM336H GPS | RX `GPIO4`, TX `GPIO5` |
| `test_buzzer.cpp` | Buzzer driver | `GPIO6` |
| `test_relay.cpp` | Relay driver | `GPIO7` |
| `test_sim_a7680c.cpp` | SIM A7680C | RX `GPIO15`, TX `GPIO16` |
| `test_mpu6050.cpp` | MPU6050 | SDA `GPIO17`, SCL `GPIO18` |
| `test_inputs.cpp` | RESET/SOS inputs | RESET `GPIO40`, SOS `GPIO39` |
| `test_all.cpp` | Combined smoke test | All pins above |

The relay and buzzer outputs are tested through their transistor drivers.
The key input is therefore driven as an output in `test_relay.cpp`.
RESET and SOS are assumed to connect to ground when pressed and use the
internal pull-up resistors.

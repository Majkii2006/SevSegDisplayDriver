# 7SegLibrary
---

Simple lightweight library made in C language for controlling the 7 segment display.

---

## Features

- Optimized for microcontrollers.
- Supports many digits.
- Default anode or kathode handling.
- Supports letters and numbers.
- Different unit like: °C, °F, %, etc.
- Constantly developing.

---
## Supported devices

- Every ESP on Xtensa CPU
- Every microcontroller using the ESP-IDF SDK

---
## Setup and usage

| API    | MEANING |
| -------- | ------- |
| display_init()  | Initialize display with specific parameters |
| display_setNumber() | Using for setting the concrete number to the buffer |
| display_setTempUnit()    | Setting the temperature unit on the display (°C, °F) |
| display_setHumidityUnit()    | Setting the humidity unit on the display (%) |
| display_worker()   | Main worker function for switching on and off the segments on the display |

---
## Example for the 2 digit LED display

``` C
SevenSegment_t display; // creating an instance of a main struct

const uint8_t segPins[] = { 14, 32, 33, 26, 25, 27, 22 }; // from A to G ordered
const uint8_t digPins[] = { 19, 23 }; // from left to right digits

display_init(&display, segPins, NUMBER_OF_SEGMENTS, digPins, NUMBER_OF_DIGITS, true); // last argument -> true = default anode controlled
display_setNumber(&display, 25);

while(1) {
  display_worker(&display);
}

```
  
## License

Licensed under the Apache License, Version 2.0.  

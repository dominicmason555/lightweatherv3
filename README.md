# Light Weather V3

![Photo of assembled project PCB](./pictures/lightweather_photo.webp)

I made a temperature/pressure/humidity/gas sensor and general I/O interface for
my "smart flat".

[Full write-up](https://domson.dev/projects/lightweather.html) of versions 1
through 3 (this version) on my [personal website](https://domson.dev).

It features an ESP32-C3 controlling two NMOS outputs, one current-limited USB A
(power only) output, and an addressable RGB LED. It has pressure, temperature,
humidity, and gas sensors, and an IR receiver. These can be controlled using
MQTT over WiFi, or using an IR remote.

- [Schematic](./pictures/lightweather_schematic.pdf)
- [Firmware](./firmware/)


# kobo-remote

Page-turner Bluetooth remote for Kobo e-readers, built with an ESP32.

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=flat&logo=espressif&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=cplusplus&logoColor=white)

## Hardware

- ESP32 with BLE (see [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) support list)
- 3 buttons

### Wiring

| Component   | Pin     |
|-------------|---------|
| Pair button | GPIO 25 |
| Up button   | GPIO 32 |
| Down button | GPIO 33 |

Pins are defined in `include/remote.h`.

## Build

```sh
pio run
pio run --target upload
```

## Usage

Press "Pair" button to forget paired device and connect a new device.

**Vanilla**

More > Settings > Bluetooth connection > Enable and pair with "Kobo Remote".

**KOReader**

Requires a plugin to enable Bluetooth on Kobo such as [bluetooth.koplugin](https://github.com/onatbas/bluetooth.koplugin) or [kobo.koplugin](https://github.com/OGKevin/kobo.koplugin).

1. Go to Network > Bluetooth.
2. Pair with "Kobo Remote".
3. Set key bindings (Next Page, Previous Page).
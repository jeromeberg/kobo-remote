#pragma once

#include <cstdint>

/* pins */
constexpr std::uint8_t UP_PIN = 32;
constexpr std::uint8_t DOWN_PIN = 33;
constexpr std::uint8_t PAIR_PIN = 25;

/* keys */
constexpr std::uint8_t UP_ARROW_KEY = 0x52;
constexpr std::uint8_t DOWN_ARROW_KEY = 0x51;

/* button */
struct Button {
    std::uint8_t pin;
    bool stableState;
    bool lastReading;
    std::uint32_t lastChange;
};

void beginButton(Button& button);
bool wasPressed(Button& button);

/* bluetooth */
void beginBluetoothHID();
void sendKey(std::uint8_t key);
void forgetPairedDevice();

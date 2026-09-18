#include <Arduino.h>
#include "remote.h"

constexpr std::uint32_t DEBOUNCE_MS = 30;

void beginButton(Button& button) {
    pinMode(button.pin, INPUT_PULLUP);
    const bool pressed = digitalRead(button.pin) == LOW;
    button.stableState = pressed;
    button.lastReading = pressed;
    button.lastChange = millis();
}

bool wasPressed(Button& button) {
    const bool reading = digitalRead(button.pin) == LOW;
    const std::uint32_t now = millis();

    if (reading != button.lastReading) {
        button.lastReading = reading;
        button.lastChange = now;
    }

    if (reading == button.stableState ||
        now - button.lastChange < DEBOUNCE_MS) {
        return false;
    }

    button.stableState = reading;
    return reading;
}

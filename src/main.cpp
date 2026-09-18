#include <Arduino.h>
#include <cstdint>
#include "remote.h"

Button upBtn{UP_PIN, false, false, 0};
Button downBtn{DOWN_PIN, false, false, 0};
Button pairBtn{PAIR_PIN, false, false, 0};

void setup() {
    beginButton(upBtn);
    beginButton(downBtn);
    beginButton(pairBtn);
    beginBluetoothHID();
}

void loop() {
    if (wasPressed(pairBtn)) {
        forgetPairedDevice();
    }

    if (wasPressed(upBtn)) {
        sendKey(UP_ARROW_KEY);
    }

    if (wasPressed(downBtn)) {
        sendKey(DOWN_ARROW_KEY);
    }

    delay(5);
}

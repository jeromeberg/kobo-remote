#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>

#include "remote.h"

constexpr std::uint8_t REPORT_ID = 1;
constexpr std::uint32_t KEY_TAP_MS = 20;

constexpr std::uint8_t HID_REPORT_DESCRIPTOR[] = {
    0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01, 0x05, 0x07, 0x19,
    0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x08,
    0x81, 0x02, 0x95, 0x01, 0x75, 0x08, 0x81, 0x01, 0x95, 0x05, 0x75,
    0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02, 0x95, 0x01,
    0x75, 0x03, 0x91, 0x01, 0x95, 0x06, 0x75, 0x08, 0x15, 0x00, 0x25,
    0x65, 0x05, 0x07, 0x19, 0x00, 0x29, 0x65, 0x81, 0x00, 0xC0,
};

struct KeyboardReport {
    std::uint8_t modifiers;
    std::uint8_t reserved;
    std::uint8_t keys[6];
};

static_assert(sizeof(KeyboardReport) == 8, "Error");

NimBLEServer* server = nullptr;
NimBLECharacteristic* inputReport = nullptr;

static void setBluetoothSecurity() {
    NimBLEDevice::setSecurityAuth(true, false, true);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
    NimBLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC |
                                     BLE_SM_PAIR_KEY_DIST_ID);
    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC |
                                     BLE_SM_PAIR_KEY_DIST_ID);
}

void beginBluetoothHID() {
    NimBLEDevice::init("Kobo Remote");

    server = NimBLEDevice::createServer();
    server->advertiseOnDisconnect(true);

    auto* hid = new NimBLEHIDDevice(server);
    inputReport = hid->getInputReport(REPORT_ID);
    hid->getOutputReport(REPORT_ID);
    hid->setPnp(0x02, 0x303A, 0x0001, 0x0100);
    hid->setHidInfo(0x00, 0x01);

    setBluetoothSecurity();
    hid->setReportMap(const_cast<std::uint8_t*>(HID_REPORT_DESCRIPTOR),
                      sizeof(HID_REPORT_DESCRIPTOR));
    hid->setBatteryLevel(100);
    server->start();

    NimBLEAdvertising* advertising = NimBLEDevice::getAdvertising();
    advertising->setAppearance(0x03C1);  // remote: 0x0180?
    advertising->addServiceUUID(hid->getHidService()->getUUID());
    advertising->start();
}

void sendKey(std::uint8_t key) {
    if (server == nullptr || server->getConnectedCount() == 0 ||
        inputReport == nullptr) {
        return;
    }

    KeyboardReport report{};
    report.keys[0] = key;
    inputReport->notify(report);

    delay(KEY_TAP_MS);
    report.keys[0] = 0;
    inputReport->notify(report);
}

void forgetPairedDevice() {
    NimBLEDevice::deleteAllBonds();
    delay(100);
    ESP.restart();
}

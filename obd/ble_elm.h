#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>

class BleElm
{
public:

    // 初期化
    bool begin();

    // 接続
    bool connect();

    // 切断
    void disconnect();

    // 接続状態
    bool isConnected() const;

    // デバッグ
    void printServices();

    // UART
    bool write(const String& data);
    bool available() const;
    String read();
    void clearRxBuffer();

    bool subscribe();

private:

    bool scan();
    bool discoverServices();
    bool discoverCharacteristics();

    NimBLEClient* client = nullptr;

    const NimBLEAdvertisedDevice* advDevice = nullptr;

    NimBLERemoteService* uartService = nullptr;

    NimBLERemoteCharacteristic* txChar = nullptr;
    NimBLERemoteCharacteristic* rxChar = nullptr;
};
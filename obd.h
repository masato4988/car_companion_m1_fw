#pragma once

#include <Arduino.h>

class BleElm;

class Obd
{
public:

    bool begin(BleElm* ble);

    bool initialize();

    String sendCommand(const String& cmd);

    float getEngineRPM();

private:

    BleElm* ble = nullptr;
};
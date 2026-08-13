#pragma once

#include <Arduino.h>

class BleElm;

class Obd
{
public:

    bool begin(BleElm* ble);

    bool initialize();

    void update();

    
    bool isIdleStop();
   
    
    float getRPM() const;
    float getSpeed() const;
    bool getIdleStop() const;
    
private:
    
    BleElm* ble = nullptr;
    
    float rpm = 0.0f;
    float speed = 0.0f;
    
    bool idleStop = false;

    String sendCommand(const String& cmd);
    float getEngineRPM();
    float getVehicleSpeed();
};
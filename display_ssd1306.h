#pragma once

#include <Arduino.h>

class DisplaySsd1306
{
public:

    bool begin();

    bool show(const uint8_t* bitmap);

    void clear();

private:

    void sendCommand(uint8_t cmd);

    void sendData(const uint8_t* data, size_t length);
};
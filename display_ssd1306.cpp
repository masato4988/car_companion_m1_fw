#include "display_ssd1306.h"

#include <Wire.h>

namespace
{
constexpr uint8_t OLED_ADDR = 0x3C;
}

bool DisplaySsd1306::begin()
{
    Wire.begin(21, 22);

    delay(100);

    static const uint8_t init[] =
    {
        0xAE,             // Display OFF

        0xD5, 0x80,       // Clock Divide
        0xA8, 0x3F,       // Multiplex = 64

        0xD3, 0x00,       // Display Offset
        0x40,             // Start Line

        0x8D, 0x14,       // Charge Pump

        0x20, 0x00,       // Horizontal Addressing Mode

        0xA1,             // Segment Remap
        0xC8,             // COM Scan Direction

        0xDA, 0x12,       // COM Pins

        0x81, 0xCF,       // Contrast

        0xD9, 0xF1,       // Pre-charge

        0xDB, 0x40,       // VCOM Detect

        0xA4,             // Resume RAM Display
        0xA6,             // Normal Display

        0x2E,             // Disable Scroll

        0xAF              // Display ON
    };

    for(uint8_t cmd : init)
    {
        sendCommand(cmd);
    }

    clear();

    Serial.println("[OLED] Initialized");

    return true;
}

bool DisplaySsd1306::show(const uint8_t* bitmap)
{
    // Column Address
    sendCommand(0x21);
    sendCommand(0);
    sendCommand(127);

    // Page Address
    sendCommand(0x22);
    sendCommand(0);
    sendCommand(7);

    size_t index = 0;

    while(index < 1024)
    {
        Wire.beginTransmission(OLED_ADDR);

        Wire.write(0x40);

        uint8_t bytes = 1;

        while(bytes < 32 && index < 1024)
        {
            Wire.write(bitmap[index++]);
            bytes++;
        }

        Wire.endTransmission();
    }

    return true;
}

void DisplaySsd1306::clear()
{
    static const uint8_t blank[1024] = {0};

    show(blank);
}

void DisplaySsd1306::sendCommand(uint8_t cmd)
{
    Wire.beginTransmission(OLED_ADDR);

    Wire.write(0x00);

    Wire.write(cmd);

    Wire.endTransmission();
}
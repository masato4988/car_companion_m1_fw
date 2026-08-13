#pragma once

#include <stdint.h>

class PrimitiveRenderer
{
public:

    static void drawRect(
        uint8_t* buffer,
        int x,
        int y,
        int w,
        int h);

    static void fillRect(
        uint8_t* buffer,
        int x,
        int y,
        int w,
        int h);

private:

    static void setPixel(
        uint8_t* buffer,
        int x,
        int y);
};
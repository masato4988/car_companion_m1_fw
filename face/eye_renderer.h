#pragma once

#include <stdint.h>

struct EyeShape
{
    int cx;
    int cy;

    int width;
    int height;

    int radius;

    int upperArc;
    int lowerArc;

    // float angle;

    int upperSlope;
    int lowerSlope;
};

class EyeRenderer
{
public:

    static void drawEye(
        uint8_t* buffer,
        const EyeShape& eye);

private:

    static void setPixel(
        uint8_t* buffer,
        int x,
        int y);

    static void clearPixel(
        uint8_t* buffer,
        int x,
        int y);

    static void fillRect(
        uint8_t* buffer,
        int x,
        int y,
        int w,
        int h);

    static void applyRoundCorner(
        uint8_t* buffer,
        int x,
        int y,
        int w,
        int h,
        int radius);

    static void applyUpperArc(
        uint8_t* buffer,
        int x,
        int y,
        int w,
        int h,
        int upperCurve);

    static void applyLowerArc(
        uint8_t* buffer,
        int x,
        int y,
        int w,
        int h,
        int lowerCurve);

    static void applyUpperSlope(
        uint8_t* buffer,
        int x,
        int y,
        int w,
        int h,
        int upperSlope);

    static void applyLowerSlope(
        uint8_t* buffer,
        int x,
        int y,
        int w,
        int h,
        int lowerSlope);
};
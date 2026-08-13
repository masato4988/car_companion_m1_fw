#pragma once

#include <stdint.h>

#include "behavior_manager.h"

class DebugOverlay
{
public:

    static void draw(
        uint8_t* buffer,
        const BehaviorManager& behavior);

private:

    static void drawBar(
        uint8_t* buffer,
        int x,
        int y,
        int width,
        int height,
        float value);
};
#include "debug_overlay.h"

#include <string.h>

#include "primitive_renderer.h"

void DebugOverlay::draw(
    uint8_t* buffer,
    const BehaviorManager& behavior)
{
    memset(buffer,0,128*64/8);

    //----------------------------------------
    // boredom
    //----------------------------------------

    drawBar(
        buffer,
        8,
        4,
        100,
        8,
        behavior.getBoredom());

    //----------------------------------------
    // excitement
    //----------------------------------------

    drawBar(
        buffer,
        8,
        16,
        100,
        8,
        behavior.getExcitement());

    //----------------------------------------
    // sleepiness
    //----------------------------------------
    
    drawBar(
        buffer,
        8,
        28,
        100,
        8,
        behavior.getSleepiness());

    //----------------------------------------
    // sleepiness
    //----------------------------------------
    
    drawBar(
        buffer,
        8,
        40,
        100,
        8,
        behavior.getBlinkDesire());
}

void DebugOverlay::drawBar(
    uint8_t* buffer,
    int x,
    int y,
    int width,
    int height,
    float value)
{
    if(value < 0.0f)
    {
        value = 0.0f;
    }

    if(value > 1.0f)
    {
        value = 1.0f;
    }

    int fillWidth =
        (int)(width * value);

    //----------------------------------------
    // 枠
    //----------------------------------------

    PrimitiveRenderer::drawRect(
        buffer,
        x,
        y,
        width,
        height);

    //----------------------------------------
    // 塗りつぶし
    //----------------------------------------

    PrimitiveRenderer::fillRect(
        buffer,
        x + 1,
        y + 1,
        fillWidth - 2,
        height - 2);
}
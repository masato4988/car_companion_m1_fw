#include "primitive_renderer.h"

void PrimitiveRenderer::setPixel(
    uint8_t* buffer,
    int x,
    int y)
{
    if(x < 0 || x >= 128)
        return;

    if(y < 0 || y >= 64)
        return;

    int index = x + (y / 8) * 128;

    buffer[index] |=
        (1 << (y % 8));
}

void PrimitiveRenderer::drawRect(
    uint8_t* buffer,
    int x,
    int y,
    int w,
    int h)
{
    for(int i = x; i < x + w; i++)
    {
        setPixel(buffer,i,y);
        setPixel(buffer,i,y+h-1);
    }

    for(int j = y; j < y + h; j++)
    {
        setPixel(buffer,x,j);
        setPixel(buffer,x+w-1,j);
    }
}

void PrimitiveRenderer::fillRect(
    uint8_t* buffer,
    int x,
    int y,
    int w,
    int h)
{
    for(int yy = y; yy < y + h; yy++)
    {
        for(int xx = x; xx < x + w; xx++)
        {
            setPixel(
                buffer,
                xx,
                yy);
        }
    }
}
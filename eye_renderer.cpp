#include "eye_renderer.h"

void EyeRenderer::drawEye(
    uint8_t* buffer,
    const EyeShape& eye)
{
    const int x = (64 + eye.cx) - eye.width / 2;
    const int y = (32 + eye.cy) - eye.height / 2;
    
    fillRect(
        buffer,
        x,
        y,
        eye.width,
        eye.height);
        
    applyRoundCorner(
        buffer,
        x,
        y,
        eye.width,
        eye.height,
        eye.radius);

    applyUpperArc(
        buffer,
        x,
        y,
        eye.width,
        eye.height,
        eye.upperArc);

    applyLowerArc(
        buffer,
        x,
        y,
        eye.width,
        eye.height,
        eye.lowerArc);
}

//private

void EyeRenderer::setPixel(
    uint8_t* buffer,
    int x,
    int y)
{
    if(x < 0 || x >= 128) return;
    if(y < 0 || y >= 64) return;

    buffer[x + (y / 8) * 128] |= (1 << (y & 7));
}

void EyeRenderer::clearPixel(
    uint8_t* buffer,
    int x,
    int y)
{
    if (x < 0 || x >= 128) return;
    if (y < 0 || y >= 64) return;

    buffer[x + (y / 8) * 128] &= ~(1 << (y & 7));
}

void EyeRenderer::fillRect(
    uint8_t* buffer,
    int x,
    int y,
    int w,
    int h)
{
    for(int yy = y; yy <= y + h; yy++)
    {
        for(int xx = x; xx <= x + w; xx++)
        {
            setPixel(buffer, xx, yy);
        }
    }
}

void EyeRenderer::applyRoundCorner(
    uint8_t* buffer,
    int x,
    int y,
    int w,
    int h,
    int radius)
{
    int r = radius;

    if (r <= 0)
        return;

    for (int dy = 0; dy <= r; dy++)
    {
        for (int dx = 0; dx <= r; dx++)
        {
            // 半径より外側なら消す
            if ((dx * dx + dy * dy) < (r * r))
                continue;

            // 左上
            clearPixel(
                buffer,
                x + r - dx,
                y + r - dy);

            // 右上
            clearPixel(
                buffer,
                x + w - r + dx,
                y + r - dy);

            // 左下
            clearPixel(
                buffer,
                x + r - dx,
                y + h - r + dy);

            // 右下
            clearPixel(
                buffer,
                x + w - r + dx,
                y + h - r + dy);
        }
    }
}

void EyeRenderer::applyUpperArc(
    uint8_t* buffer,
    int x,
    int y,
    int w,
    int h,
    int upperCurve)
{
    if (upperCurve <= 0)
        return;

    for (int xx = 0; xx < w; xx++)
    {
        // -1 ～ +1 に正規化
        float t = (xx - (w - 1) * 0.5f) / ((w - 1) * 0.5f);

        // 放物線
        int offset = (int)(upperCurve * t * t + 0.5f);

        // offset の高さまで削る
        for (int yy = 0; yy < offset; yy++)
        {
            clearPixel(
                buffer,
                x + xx,
                y + yy);
        }
    }
}

void EyeRenderer::applyLowerArc(
    uint8_t* buffer,
    int x,
    int y,
    int w,
    int h,
    int lowerCurve)
{
    if (lowerCurve == 0)
        return;

    // ------------------------------
    // 下に凸（従来）
    // ------------------------------
    if (lowerCurve > 0)
    {
        for (int xx = 0; xx < w; xx++)
        {
            float t = (xx - (w - 1) * 0.5f) / ((w - 1) * 0.5f);

            int offset = (int)(lowerCurve * t * t + 0.5f);

            for (int yy = 0; yy <= offset; yy++)
            {
                clearPixel(
                    buffer,
                    x + xx,
                    y + h - 1 - yy);
            }
        }
    }
    // ------------------------------
    // 上に凸（笑顔）
    // ------------------------------
    else
    {
        int curve = -lowerCurve;

        for (int xx = 0; xx <= w; xx++)
        {
            float t = (xx - (w - 1) * 0.5f) / ((w - 1) * 0.5f);

            int offset = (int)(curve * t * t + 0.5f);

            // 左右では下端、中央では curve ピクセル上になる
            int curveY = y + h - 1 - (curve - offset);

            for (int yy = curveY; yy <= y + h; yy++)
            {
                clearPixel(
                    buffer,
                    x + xx,
                    yy);
            }
        }
    }
}
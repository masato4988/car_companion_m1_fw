#include "display_ssd1306.h"
#include "eye_renderer.h"

DisplaySsd1306 oled;

uint8_t buffer[128 * 64 / 8];

void setup()
{
    Serial.begin(115200);

    if (!oled.begin())
    {
        Serial.println("OLED init failed");
        while (1);
    }
    
    memset(buffer, 0x00, sizeof(buffer));
    
}

void loop()
{
    EyeShape eye;

    for (int i = 0; i < 1024; i++){
        buffer[i] = 0;
    }
    
    eye.cx = -30;
    eye.cy = 0;
    
    eye.width = 40;
    eye.height = 30;
    
    eye.radius = 10;
    
    eye.upperArc = 0;
    eye.lowerArc = 0;
    
    eye.upperSlope = 0;
    eye.lowerSlope = 0;

    EyeRenderer::drawEye(buffer, eye);
    
    eye.cx = 30;

    EyeRenderer::drawEye(buffer, eye);
    
    oled.show(buffer);
    
    delay(950);
    
    for (int i = 0; i < 1024; i++){
        buffer[i] = 0;
    }
    eye.cx = -30;
    eye.cy = 0;
    
    eye.width = 40;
    eye.height = 30;
    
    eye.radius = 10;
    
    // eye.upperArc = 10;
    // eye.lowerArc = -10;

    eye.upperSlope = 10;
    eye.lowerSlope = -10;
    
    EyeRenderer::drawEye(buffer, eye);
    
    eye.cx = 30;

    eye.upperSlope = -10;
    eye.lowerSlope = 10;
    
    EyeRenderer::drawEye(buffer, eye);
    
    oled.show(buffer);
    
    delay(500);
}
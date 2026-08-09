#include "display_ssd1306.h"
#include "eye_renderer.h"
#include "face_animator.h"

DisplaySsd1306 oled;
FaceAnimator face;

uint8_t buffer[128 * 64 / 8];

constexpr uint32_t PERIOD = 16;

void setup()
{
    Serial.begin(115200);

    if (!oled.begin())
    {
        Serial.println("OLED init failed");
        while (1);
    }

    face.begin();
    
    // memset(buffer, 0x00, sizeof(buffer));
    
}

void loop()
{
    static uint32_t prevAnim = millis();
    static uint32_t prevExpression = millis();

    static int expressionIndex = 0;

    uint32_t now = millis();

    //----------------------------------------
    // FaceAnimator (100Hz)
    //----------------------------------------

    if (now - prevAnim >= 10)
    {
        prevAnim += 10;

        face.update(0.010f);

        memset(buffer, 0, sizeof(buffer));

        EyeRenderer::drawEye(buffer, face.getLeftEye());
        EyeRenderer::drawEye(buffer, face.getRightEye());

        oled.show(buffer);
    }

    //----------------------------------------
    // 1秒ごとに表情切替
    //----------------------------------------

    if (now - prevExpression >= 2000)
    {
        prevExpression += 2000;

        switch (expressionIndex)
        {
        case 0:
            Serial.println("Neutral");
            face.setExpression(
                Expression::Neutral,
                1.0f,
                0.3f);
            break;

        case 1:
            Serial.println("Smile");
            face.setExpression(
                Expression::Smile,
                1.0f,
                0.3f);
            break;

        case 2:
            Serial.println("Happy");
            face.setExpression(
                Expression::Happy,
                1.0f,
                0.3f);
            break;

        case 3:
            Serial.println("Sleepy");
            face.setExpression(
                Expression::Sleepy,
                1.0f,
                0.3f);
            break;

        case 4:
            Serial.println("Angry");
            face.setExpression(
                Expression::Angry,
                1.0f,
                0.3f);
            break;

        case 5:
            Serial.println("Sad");
            face.setExpression(
                Expression::Sad,
                1.0f,
                0.3f);
            break;

        case 6:
            Serial.println("Surprise");
            face.setExpression(
                Expression::Surprise,
                1.0f,
                0.3f);
            break;
        }

        expressionIndex++;

        if (expressionIndex >= 7)
        {
            expressionIndex = 0;
        }
    }
}
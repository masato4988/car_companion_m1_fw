#include "ble_elm.h"
#include "obd.h"
#include "display_ssd1306.h"
#include "eye_renderer.h"
#include "debug_overlay.h"
#include "companion_controller.h"

BleElm ble;
Obd obd;
DisplaySsd1306 oled;
CompanionController companion;

uint8_t buffer[128 * 64 / 8];

void setup()
{
    Serial.begin(115200);

    oled.begin();

    ble.begin();

    while(!ble.connect())
    {
        Serial.println("Retry...");
        delay(1000);
    }

    obd.begin(&ble);

    if(!obd.initialize())
    {
        Serial.println("OBD init failed");

        while(1);
    }

    companion.begin();
}

void loop()
{
    //----------------------------------------
    // 10ms周期
    //----------------------------------------

    static uint32_t prev = millis();

    uint32_t now = millis();

    if(now - prev < 10)
    {
        return;
    }

    prev += 10;

    //----------------------------------------
    // OBD更新
    //----------------------------------------

    obd.update();

    //----------------------------------------
    // Companion更新
    //----------------------------------------

    companion.update(
        obd.getRPM(),
        obd.getSpeed(),
        0.0f,                  // 後で getAccel() に変更
        obd.getIdleStop(),
        0.01f);

    //----------------------------------------
    // 描画
    //----------------------------------------

    memset(buffer, 0, sizeof(buffer));

    if(companion.isDebugMode())
    {
        DebugOverlay::draw(
            buffer,
            companion.getBehaviorManager());
    }
    else
    {
        EyeRenderer::drawEye(
            buffer,
            companion.getLeftEye());

        EyeRenderer::drawEye(
            buffer,
            companion.getRightEye());
    }

    oled.show(buffer);

    //----------------------------------------
    // デバッグ表示（1秒毎）
    //----------------------------------------

    static uint32_t prevPrint = millis();

    if(now - prevPrint >= 1000)
    {
        prevPrint += 1000;

        Serial.print("RPM = ");
        Serial.print(obd.getRPM());

        Serial.print("  Speed = ");
        Serial.print(obd.getSpeed());

        Serial.print("  IdleStop = ");
        Serial.println(obd.getIdleStop());
    }
}
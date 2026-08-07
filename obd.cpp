#include "obd.h"
#include "ble_elm.h"

bool Obd::begin(BleElm* ble_)
{
    ble = ble_;
    return (ble != nullptr);
}

bool Obd::initialize()
{
    if (ble == nullptr)
        return false;

    // BLE UART受信開始
    if (!ble->subscribe())
        return false;

    delay(200);

    // エコーOFF
    if (!sendCommand("ATE0"))
        return false;

    delay(200);

    return true;
}

String Obd::sendCommand(const String& cmd)
{
    if (ble == nullptr)
        return "";

    ble->clearRxBuffer();

    if (!ble->write(cmd))
        return "";

    uint32_t start = millis();
    String response;

    while (millis() - start < 1000)
    {
        if (ble->available())
        {
            response += ble->read();

            if (response.indexOf(">") >= 0)
            {
                return response;
            }
        }

        delay(1);
    }

    return "";
}

float Obd::getEngineRPM()
{
    // PID 0x0C : Engine RPM
    String response = sendCommand("010C");

    Serial.print("[OBD] Response : ");
    Serial.println(response);

    int index = response.indexOf("41 0C");

    if (index < 0)
    {
        Serial.println("[OBD] RPM response error");
        return -1.0f;
    }

    unsigned int A;
    unsigned int B;

    sscanf(response.c_str() + index, "41 0C %x %x", &A, &B);

    float rpm = ((A * 256) + B) / 4.0f;

    return rpm;
}
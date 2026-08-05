#include "ble_elm.h"

BleElm ble;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("===== Car Companion =====");

    if (!ble.begin())
    {
        Serial.println("[MAIN] BLE init failed");
        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("[MAIN] BLE initialized");

    if (!ble.connect())
    {
        Serial.println("[MAIN] BLE connect failed");
        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("[MAIN] BLE connected");


    ble.printServices();
    
    ble.subscribe();

    delay(500);
    
    ble.write("ATI\r");
}

void loop()
{
    delay(1000);
}
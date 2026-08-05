#include "ble_elm.h"

#include <Arduino.h>
#include <NimBLEDevice.h>

namespace {

static const char* TARGET_MAC = "bc:fd:fd:88:dd:9f";

const NimBLEAdvertisedDevice* g_advDevice = nullptr;
bool g_found = false;

class ClientCallbacks : public NimBLEClientCallbacks
{
    void onConnect(NimBLEClient*) override
    {
        Serial.println("[BLE] Connected");
    }

    void onDisconnect(NimBLEClient*, int reason) override
    {
        Serial.printf("[BLE] Disconnected (reason=%d)\n", reason);
    }
};

ClientCallbacks g_callbacks;

class ScanCallbacks : public NimBLEScanCallbacks
{
    void onResult(const NimBLEAdvertisedDevice* dev) override
    {
        Serial.printf("[SCAN] %s\n", dev->toString().c_str());

        String addr = dev->getAddress().toString().c_str();
        addr.toLowerCase();

        if (addr == TARGET_MAC)
        {
            Serial.println("[SCAN] ELM327 FOUND");
            g_advDevice = dev;
            g_found = true;
            NimBLEDevice::getScan()->stop();
        }
    }
};

ScanCallbacks g_scanCallbacks;


void notifyCallback(
    NimBLERemoteCharacteristic* chr,
    uint8_t* data,
    size_t length,
    bool isNotify)
{
    Serial.print("[BLE] RX : ");

    for (size_t i = 0; i < length; i++)
    {
        Serial.print((char)data[i]);
    }

    Serial.println();
}

} // namespace

//public

bool BleElm::begin()
{
    Serial.println("[BLE] Init");

    NimBLEDevice::init("CarCompanion");
    NimBLEDevice::setPower(3);

    NimBLEScan* scan = NimBLEDevice::getScan();
    scan->setScanCallbacks(&g_scanCallbacks, false);
    scan->setActiveScan(true);
    scan->setInterval(100);
    scan->setWindow(100);

    return true;
}

bool BleElm::connect()
{
    if (!scan())
    {
        Serial.println("[BLE] ELM327 not found");
        return false;
    }

    client = NimBLEDevice::createClient();

    client->setClientCallbacks(&g_callbacks, false);
    client->setConnectTimeout(5000);

    Serial.println("[BLE] Connecting...");

    if (!client->connect(g_advDevice))
    {
        Serial.println("[BLE] Connect failed");

        NimBLEDevice::deleteClient(client);

        client = nullptr;

        return false;
    }

    Serial.println("[BLE] Connect success");
    Serial.printf("[BLE] RSSI = %d\n", client->getRssi());

    return true;
}

void BleElm::printServices()
{
    if (client == nullptr)
    {
        Serial.println("[BLE] Client is null");
        return;
    }

    Serial.println();
    Serial.println("===== SERVICES =====");

    const std::vector<NimBLERemoteService*>& services =
        client->getServices(true);

    if (services.empty())
    {
        Serial.println("[BLE] No services found");
        return;
    }

    for (auto service : services)
    {
        Serial.print("Service : ");
        Serial.println(service->getUUID().toString().c_str());

        const std::vector<NimBLERemoteCharacteristic*>& chars =
            service->getCharacteristics(true);

        for (auto chr : chars)
        {
            Serial.print("    Characteristic : ");
            Serial.print(chr->getUUID().toString().c_str());

            Serial.print("   ");

            if (chr->canRead())   Serial.print("[R]");
            if (chr->canWrite())  Serial.print("[W]");
            if (chr->canNotify()) Serial.print("[N]");
            if (chr->canIndicate()) Serial.print("[I]");


            Serial.println();
        }

        // if (service->getUUID().toString() == "0xfff0")
        // {
        //     const auto& chars = service->getCharacteristics(true);

        //     for (auto chr : chars)
        //     {
        //         if (chr->getUUID().toString() == "0xfff2")
        //         {
        //             txChar = chr;
        //             Serial.println("[BLE] TX Characteristic found");
        //         }

        //         if (chr->getUUID().toString() == "0xfff1")
        //         {
        //             rxChar = chr;
        //             Serial.println("[BLE] RX Characteristic found");
        //         }
        //     }
        // }

        Serial.println();
    }
}

bool BleElm::write(const String& data)
{
    if (client == nullptr)
    {
        Serial.println("[BLE] write() : client is null");
        return false;
    }

    if (txChar == nullptr)
    {
        Serial.println("[BLE] write() : txChar is null");
        return false;
    }

    Serial.print("[BLE] TX : ");
    Serial.println(data);

    bool result = txChar->writeValue(
        (uint8_t*)data.c_str(),
        data.length(),
        false);

    if (!result)
    {
        Serial.println("[BLE] write failed");
        return false;
    }

    Serial.println("[BLE] write success");

    return true;
}

bool BleElm::subscribe()
{
    if (rxChar == nullptr)
    {
        Serial.println("[BLE] RX characteristic not found");
        return false;
    }

    if (!rxChar->canNotify())
    {
        Serial.println("[BLE] RX cannot notify");
        return false;
    }

    if (!rxChar->subscribe(true, notifyCallback))
    {
        Serial.println("[BLE] Subscribe failed");
        return false;
    }

    Serial.println("[BLE] Subscribe success");

    return true;
}

//private

bool BleElm::scan()
{
    g_found = false;

    Serial.println("[BLE] Scanning...");

    NimBLEDevice::getScan()->start(5000);

    // 見つかるまで待つ
    uint32_t startTime = millis();

    while (!g_found)
    {
        delay(10);

        if (millis() - startTime > 5000)
        {
            return false;
        }
    }

    return true;
}
#include "ble_elm.h"

#include <Arduino.h>
#include <NimBLEDevice.h>

namespace {

static const char* TARGET_MAC = "bc:fd:fd:88:dd:9f";

const NimBLEAdvertisedDevice* g_advDevice = nullptr;
bool g_found = false;
String g_rxBuffer;

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
    Serial.println("========== NOTIFY ==========");

    Serial.printf("Length = %u\n", length);

    Serial.print("HEX : ");
    for (size_t i = 0; i < length; i++)
    {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();

    Serial.print("ASCII : ");
    for (size_t i = 0; i < length; i++)
    {
        Serial.print((char)data[i]);

        // ★受信バッファへ追加
        g_rxBuffer += (char)data[i];
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

    if (!discoverServices())
    {
        return false;
    }
    if (!discoverCharacteristics())
    
    {
        return false;
    }

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

    const auto& services = client->getServices(true);

    if (services.empty())
    {
        Serial.println("[BLE] No services found");
        return;
    }

    for (auto service : services)
    {
        Serial.print("Service : ");
        Serial.println(service->getUUID().toString().c_str());
        const auto& chars = service->getCharacteristics(true);

        for (auto chr : chars)
        {
            Serial.print("    Characteristic : ");
            Serial.print(chr->getUUID().toString().c_str());

            Serial.print("   ");

            if (chr->canRead())     Serial.print("[R]");
            if (chr->canWrite())    Serial.print("[W]");
            if (chr->canNotify())   Serial.print("[N]");
            if (chr->canIndicate()) Serial.print("[I]");

            Serial.println();
        }

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
        true);

    if (!result)
    {
        Serial.println("[BLE] write failed");
        return false;
    }

    Serial.println("[BLE] write success");

    return true;
}

bool BleElm::available() const
{
    return !g_rxBuffer.isEmpty();
}

String BleElm::read()
{
    String str = g_rxBuffer;
    g_rxBuffer = "";
    return str;
}

void BleElm::clearRxBuffer()
{
    g_rxBuffer = "";
}

bool BleElm::subscribe()
{
    if (rxChar == nullptr)
    {
        Serial.println("[BLE] RX characteristic not found");
        return false;
    }

    // if (!rxChar->canNotify())
    // {
    //     Serial.println("[BLE] RX cannot notify");
    //     return false;
    // }

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

bool BleElm::discoverServices()
{
    if (client == nullptr)
    {
        Serial.println("[BLE] Client is null");
        return false;
    }

    uartService = nullptr;

    const auto& services = client->getServices(true);

    if (services.empty())
    {
        Serial.println("[BLE] No services");
        return false;
    }

    for (auto service : services)
    {
        String uuid = service->getUUID().toString().c_str();
        uuid.toLowerCase();

        Serial.print("[BLE] Check Service : ");
        Serial.println(uuid);

        if (uuid == "0xfff0")
        {
            uartService = service;

            Serial.println("[BLE] UART Service Found");

            return true;
        }
    }

    Serial.println("[BLE] UART Service Not Found");

    return false;
}

bool BleElm::discoverCharacteristics()
{
    if (uartService == nullptr)
    {
        Serial.println("[BLE] UART Service is null");
        return false;
    }

    txChar = nullptr;
    rxChar = nullptr;

    const auto& chars = uartService->getCharacteristics(true);

    if (chars.empty())
    {
        Serial.println("[BLE] No Characteristics");
        return false;
    }

    for (auto chr : chars)
    {
        String uuid = chr->getUUID().toString().c_str();
        uuid.toLowerCase();

        Serial.print("[BLE] Check Characteristic : ");
        Serial.println(uuid);

        if (uuid == "0xfff1")
        {
            rxChar = chr;
            Serial.println("[BLE] RX Characteristic Found");
        }

        if (uuid == "0xfff2")
        {
            txChar = chr;
            Serial.println("[BLE] TX Characteristic Found");
        }
    }

    if (txChar == nullptr)
    {
        Serial.println("[BLE] TX Characteristic Not Found");
        return false;
    }

    if (rxChar == nullptr)
    {
        Serial.println("[BLE] RX Characteristic Not Found");
        return false;
    }

    return true;
}
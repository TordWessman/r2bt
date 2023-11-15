#include "R2BTHub.h"

R2BTHub::R2BTHub(const char *name, const char *serviceId)
{
    NimBLEDevice::init(name);
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    server = NimBLEDevice::createServer();
    service = server->createService(serviceId);
    advertising = NimBLEDevice::getAdvertising();
    advertising->addServiceUUID(serviceId);
    advertising->setScanResponse(true);
    advertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    advertising->setMinPreferred(0x12);
}

void R2BTHub::AddDataContainer(R2BTDataContainer *dataContainer, cJSON *initialValue)
{
    R2BTCallbackHandler *callbackHandler = new R2BTCallbackHandler(dataContainer);
    callbackHandlers.push_back(callbackHandler);
    NimBLECharacteristic *characteristic = dataContainer->Instantiate(service);
    char *json_str = cJSON_Print(initialValue);
    characteristic->setValue((uint8_t*)json_str, strlen(json_str) + 1);
    characteristic->setCallbacks((NimBLECharacteristicCallbacks *)callbackHandler);
    characteristic->notify();
}

void R2BTHub::Start()
{
    service->start();
    BLEDevice::startAdvertising();
}

void R2BTHub::Stop()
{
    server->stopAdvertising();
    NimBLEDevice::deinit();
}
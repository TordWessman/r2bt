#include "R2BTHub.h"

R2BTHub::R2BTHub(const char *name, const char *serviceId)
{
    BLEDevice::init(name);
    pServer = BLEDevice::createServer();
    pService = pServer->createService(serviceId);
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(serviceId);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
}

void R2BTHub::AddDataContainer(R2BTDataContainer *dataContainer, cJSON *initialValue)
{
    R2BTCallbackHandler *callbackHandler = new R2BTCallbackHandler(dataContainer);
    callbackHandlers.push_back(callbackHandler);
    BLECharacteristic *characteristic = dataContainer->Instantiate(pService);
    char *json_str = cJSON_Print(initialValue);
    characteristic->setValue(json_str);
    characteristic->setCallbacks((BLECharacteristicCallbacks *)callbackHandler);
    free(json_str);
}

void R2BTHub::Start()
{
    pService->start();
    BLEDevice::startAdvertising();
}

void R2BTHub::Stop()
{
    pService->stop();
    BLEDevice::stopAdvertising();
}
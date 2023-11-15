#ifndef __R2BTCallbackHandler_H__
#define __R2BTCallbackHandler_H__

#include <NimBLEDevice.h>

#include "R2BTDataContainer.h"

class R2BTCallbackHandler : NimBLECharacteristicCallbacks
{

public:
    R2BTCallbackHandler(R2BTDataContainer *dataContainer) : dataContainer(dataContainer) {}

    ~R2BTCallbackHandler() override {}

    void onWrite(NimBLECharacteristic *characteristic) override;

    void onRead(NimBLECharacteristic *characteristic) override;

private:
    R2BTDataContainer *dataContainer;
};
#endif
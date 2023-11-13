#ifndef __R2BTCallbackHandler_H__
#define __R2BTCallbackHandler_H__

#include <BLEDevice.h>

#include "R2BTDataContainer.h"

class R2BTCallbackHandler : BLECharacteristicCallbacks
{

public:
    R2BTCallbackHandler(R2BTDataContainer *dataContainer) : dataContainer(dataContainer) {}

    ~R2BTCallbackHandler() override {}

    void onWrite(BLECharacteristic *characteristic) override;

    void onRead(BLECharacteristic *characteristic) override;

private:
    R2BTDataContainer *dataContainer;
};
#endif
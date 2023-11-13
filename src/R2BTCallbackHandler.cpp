
#include "R2BTCallbackHandler.h"
#include "cJSON.h"
#include "R2BTError.h"

void R2BTCallbackHandler::onWrite(BLECharacteristic *characteristic)
{
    cJSON *message = cJSON_Parse(characteristic->getValue().c_str());

    if (message == NULL)
    {
        const char *errorPtr = cJSON_GetErrorPtr();
        R2BTError *error = new R2BTError(R2BTError::Type::InvalidJSONReceived, errorPtr);
        dataContainer->OnUpdate(NULL, error);
        delete error;
    }
    else
    {
        dataContainer->OnUpdate(message, NULL);
        cJSON_Delete(message);
    }
}

void R2BTCallbackHandler::onRead(BLECharacteristic *characteristic)
{
    dataContainer->OnRead();
}
#include "R2BTDataContainer.h"

#include <cstring>

R2BTDataContainer::R2BTDataContainer(uint16_t id, R2BTDataContainerAccessMode accessMode) : id(id), accessMode(accessMode), characteristic(nullptr)
{
    characteristicId[UUID_LENGTH] = '\n';
    std::strcpy(characteristicId, R2BTDataContainer_CHARACTERISTICS_ID_TEMPLATE);
    char cId[5];
    snprintf(cId, 5, "%04X", id);
    memcpy(characteristicId, cId, 4);
}

char* R2BTDataContainer::GenerateRawValue()
{
    assert(characteristic != nullptr); // The endpoint must have been added before value can be accessed.

    const char *currentValue = characteristic->getValue().c_str();
    char *returnValue = new char[strlen(currentValue) + 1];
    memcpy(returnValue, currentValue, strlen(currentValue));
    returnValue[strlen(currentValue)] = '\0';
   
    return returnValue;
}

cJSON* R2BTDataContainer::GenerateValue()
{
    return cJSON_Parse(characteristic->getValue().c_str());
}

void R2BTDataContainer::SetValue(cJSON *newValue)
{
    char *json_str = cJSON_Print(newValue);
    characteristic->setValue(json_str);
    free(json_str);
    characteristic->notify();
}

BLECharacteristic* R2BTDataContainer::Instantiate(BLEService *pService)
{
    assert(characteristic == nullptr);

    characteristic = pService->createCharacteristic(characteristicId, accessMode);

    return characteristic;
}
#ifndef __R2BTDataContainer_H__
#define __R2BTDataContainer_H__

#include <NimBLEDevice.h>

#include "R2BTError.h"
#include "cJSON.h"

class R2BTHub;

#ifndef R2BTDataContainer_CHARACTERISTICS_ID_TEMPLATE

/// For simplification, a "default UUID" is being used. Device is only identified by a 16-bit int.
#define R2BTDataContainer_CHARACTERISTICS_ID_TEMPLATE "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa"
#endif

#define UUID_LENGTH 36

enum R2BTDataContainerAccessMode
{
    R2BTEndpointAccessModeRead = NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY,
    R2BTEndpointAccessModeWrite = NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR,
    R2BTEndpointAccessModeReadWrite = NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::NOTIFY
};

/// @brief Represents a "variable" that can be accessed by clients.
class R2BTDataContainer
{

public:

    ~R2BTDataContainer() { }

    /// @brief For simplification, a "default UUID" is being used. Device is only identified by a 16-bit int.
    /// @param id
    /// @param accessMode
    R2BTDataContainer(uint16_t id, R2BTDataContainerAccessMode accessMode = R2BTEndpointAccessModeReadWrite);

    /// @brief Callet when a new value has been received
    /// @param newValue will be `nullptr` on errors.
    /// @param error will be `nullptr` if successfull.
    virtual void OnUpdate(cJSON *newValue, R2BTError *error) { }

    /// @brief Called whenever a value is read.
    virtual void OnRead() { }

    /// @brief Returns the simplified id
    /// @return 
    uint16_t GetId() { return id; }

    /// @brief Allocates and returns a raw representation of the data. Release after use.
    /// @return 
    char *GenerateRawValue();

    /// @brief Allocates and generates the JSON representation of the data. Release after use.
    /// @return 
    cJSON *GenerateValue();

    /// @brief Updates internal value and broadcast.
    /// @param newValue 
    void SetValue(cJSON *newValue);

    R2BTDataContainer &operator=(const R2BTDataContainer &) = delete;
    R2BTDataContainer(const R2BTDataContainer &other) = delete;

    friend class R2BTHub;

private:
    NimBLECharacteristic *Instantiate(NimBLEService *service);
    const uint16_t id;
    R2BTDataContainerAccessMode accessMode;
    NimBLECharacteristic *characteristic;

    char characteristicId[UUID_LENGTH + 1];
};

#endif
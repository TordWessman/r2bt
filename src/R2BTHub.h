#ifndef __r2_bt_hub_h__
#define __r2_bt_hub_h__

#include <NimBLEDevice.h>

#include <vector>

#include "R2BTDataContainer.h"
#include "R2BTCallbackHandler.h"
#include "cJSON.h"

/// @brief BLE interface
class R2BTHub
{

public:

    /// @brief Creates a new instance. Call `Start` to start broadcasting.
    /// @param name the human-readable name of the device
    /// @param serviceId the service-UUID of the device.
    R2BTHub(const char *name, const char *serviceId);

    ~R2BTHub()
    {
        delete service;
        delete advertising;
        for (R2BTCallbackHandler *callbackHandler : callbackHandlers)
        {
            delete callbackHandler;
        }
    }

    /// @brief Adds a `R2BTDataContainer` and make it publicly available.
    /// @param dataContainer object to be accessed.
    /// @param initialValue objects initial value.
    void AddDataContainer(R2BTDataContainer *dataContainer, cJSON *initialValue);

    /// @brief Start broadcasting
    void Start();

    /// @brief Stop broadcasting
    void Stop();

    R2BTHub &operator=(const R2BTHub &) = delete;
    R2BTHub(const R2BTHub &other) = delete;

    

private:
    NimBLEServer *server;
    NimBLEService *service;
    NimBLEAdvertising *advertising;
    std::vector<R2BTCallbackHandler*> callbackHandlers;
};

#endif
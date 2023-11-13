# R2BT
Simple Bluetooth Low Energy(BLE) abstraction library (tested for ESP-32).
It's intended to be used as a "variable storage" where a `R2BTDataContainer` represents a "variable".

Communicates using JSON-packages and is coupled to the `cJSON` library (https://github.com/DaveGamble/cJSON).

Example blob:

```
#include <r2bt.h>

/// Override to access the delegate methods.
class AVariable: public R2BTDataContainer {

public:
    AVariable(uint16_t id) : R2BTDataContainer(id) { }

    // Callback when a container has been updated.
    void OnUpdate(cJSON *newValue, R2BTError *error) override {
        Serial.println("Got updated, yo!");  
    }
};

AVariable *aVariable;
R2BTHub *hub;
#define A_SERVICE_ID "aaaabbbb-1fb5-459e-8fcc-c5c9c331914b"

void setup() {
    Serial.begin(9600);

    aVariable = new AVariable(0x42); // The full characteristics id will be defined by `R2BTDataContainer_CHARACTERISTICS_ID_TEMPLATE`. `id` only denotes the first 4 bytes.

    hub = new R2BTHub("device_name", A_SERVICE_ID); // Device will be identified by `"device_name"` and `A_SERVICE_ID`

    // Create a cJSON object
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "cause_of_death", "death by eating");

    // Instantiate the variable by adding it to the hub.
    hub->AddDataContainer(aVariable, json);

    // Free up the JSON object
    cJSON_Delete(json);

    hub->Start();
}

void loop() {

    //Code below will update (and broadcast) the value every 10 seconds:

    delay(10 * 1000);
    cJSON *newValue = cJSON_CreateObject();
    cJSON_AddStringToObject(newValue, "cause_of_death", "death by fishing");

    // Print current value
    char *currentValue = testEp->GenerateRawValue();
    Serial.print("Current value (before change): "); Serial.println(currentValue);
    free(currentValue);

    //Broadcast new value
    aVariable->SetValue(newValue);

    cJSON_Delete(newValue);
}
```
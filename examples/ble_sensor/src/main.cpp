/**
 * r2bt Example: BLE Sensor Hub
 *
 * Demonstrates how to use r2bt to expose sensor-like data over
 * Bluetooth Low Energy. Two data containers are created:
 *
 *   - TemperatureSensor (read-only, notifies subscribers every 5 s)
 *   - LedSwitch        (read-write, lets a BLE client toggle an LED)
 *
 * Connect with any BLE scanner (e.g. nRF Connect) to read/write
 * the characteristics. Values are exchanged as JSON strings.
 */

#include <Arduino.h>
#include <r2bt.h>

// ---- Configuration --------------------------------------------------------

#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"
#define DEVICE_NAME  "r2bt-sensor"

// Characteristic IDs (only the first 4 hex digits of the UUID matter)
#define TEMP_ID  0x0001
#define LED_ID   0x0002

#define LED_PIN 2  // Built-in LED on most ESP32 boards

// ---- Custom data containers -----------------------------------------------

// Read-only temperature "sensor" that broadcasts a value periodically.
class TemperatureSensor : public R2BTDataContainer {
public:
    TemperatureSensor() : R2BTDataContainer(TEMP_ID, R2BTEndpointAccessModeRead) {}

    void OnRead() override {
        Serial.println("[temp] Client read the temperature value");
    }

    // Call this to push a new reading to subscribed clients.
    void publish(float celsius) {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "celsius", celsius);
        SetValue(json);
        cJSON_Delete(json);
    }
};

// Read-write LED switch. A BLE client can write {"on": true/false}.
class LedSwitch : public R2BTDataContainer {
public:
    LedSwitch() : R2BTDataContainer(LED_ID, R2BTEndpointAccessModeReadWrite) {}

    void OnUpdate(cJSON *newValue, R2BTError *error) override {
        if (error) {
            Serial.printf("[led] Error: %s\n", error->message);
            return;
        }

        cJSON *onItem = cJSON_GetObjectItem(newValue, "on");
        if (cJSON_IsBool(onItem)) {
            bool on = cJSON_IsTrue(onItem);
            digitalWrite(LED_PIN, on ? HIGH : LOW);
            Serial.printf("[led] LED turned %s\n", on ? "ON" : "OFF");
        } else {
            Serial.println("[led] Expected JSON: {\"on\": true/false}");
        }
    }
};

// ---- Global instances -----------------------------------------------------

TemperatureSensor *tempSensor;
LedSwitch *ledSwitch;
R2BTHub *hub;

// ---- Arduino lifecycle ----------------------------------------------------

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("r2bt BLE Sensor example starting...");

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Create data containers
    tempSensor = new TemperatureSensor();
    ledSwitch  = new LedSwitch();

    // Create the BLE hub
    hub = new R2BTHub(DEVICE_NAME, SERVICE_UUID);

    // Set the initial temperature value
    cJSON *initTemp = cJSON_CreateObject();
    cJSON_AddNumberToObject(initTemp, "celsius", 20.0);
    hub->AddDataContainer(tempSensor, initTemp);
    cJSON_Delete(initTemp);

    // Set the initial LED state
    cJSON *initLed = cJSON_CreateObject();
    cJSON_AddBoolToObject(initLed, "on", false);
    hub->AddDataContainer(ledSwitch, initLed);
    cJSON_Delete(initLed);

    // Start advertising
    hub->Start();
    Serial.println("BLE advertising started. Connect with a BLE scanner.");
}

void loop() {
    // Simulate a temperature reading every 5 seconds
    static unsigned long lastPublish = 0;
    if (millis() - lastPublish >= 5000) {
        lastPublish = millis();

        float fakeCelsius = 20.0 + (random(0, 100) / 10.0);
        tempSensor->publish(fakeCelsius);

        char *raw = tempSensor->GenerateRawValue();
        Serial.printf("[temp] Published: %s\n", raw);
        delete[] raw;
    }
}

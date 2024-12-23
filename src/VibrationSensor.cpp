#include "header/Vibration_Sensor.h"
#include "header/MqttPublisher.h"

#include <cJSON.h>

float VibrationSensor::Get_Vibrant_Intensity()
{
    float value = analogRead(Analog_Pin);
    Serial.println(value);
    return value;
}
bool VibrationSensor::Is_Vibrant()
{
    if (is_Digital)
        return digitalRead(Digital_PIN) == LOW;
    else
        return Get_Vibrant_Intensity() == 1023;
}

void VibrationSensor::Detect_Vibrant()
{
    unsigned long currentTime = millis();
    if (Is_Vibrant() && (currentTime - lastCheckTime >= 10000 || lastCheckTime == 0) && servo->get_Is_locked())
    {
        lastState = true;
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "isShaking", "1");
        char *json_string = cJSON_Print(json);
        MqttPublisher *client = MqttPublisher::getInstance();
        client->publishMessage("vibration", json_string);
        buzzer->Sound(3000);
        lastCheckTime = currentTime;

        // send MQTT here
    }
    else
    {
        if (lastState == true)
        {
            lastState = false;
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "isShaking", "0");
            char *json_string = cJSON_Print(json);
            MqttPublisher *client = MqttPublisher::getInstance();
            client->publishMessage("vibration", json_string);
        }
    }
}
#include "header/ultraSonic.h"
#include "header/MqttPublisher.h"

#include <cJSON.h>
void UltraSonic::Setup()
{
    pinMode(Trigger_PIN, OUTPUT);
    pinMode(Echo_PIN, INPUT);
}
void UltraSonic::Detect_object()
{
    // unsigned long currentTime = millis();
    float distance = GetDistance_cm();
    if (distance == 0)
        return;
    if (distance <= 40)
    {
        if (Ison)
            return;
        Ison = true;
        digitalWrite(Led_Pin, HIGH);
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "isOn", "1");
        cJSON_AddStringToObject(json, "isDetected", "1");
        char *json_string = cJSON_Print(json);
        MqttPublisher::getInstance()->publishMessage("led", json_string);

        cJSON *json1 = cJSON_CreateObject();
        cJSON_AddStringToObject(json1, "isDetected", "1");
        char *json_string1 = cJSON_Print(json1);
        MqttPublisher::getInstance()->publishMessage("ultrasonic", json_string1);
    }
    else if (distance > 40)
    {
        if (!Ison)
            return;

        Ison = false;
        digitalWrite(Led_Pin, LOW);
        lastState = false;
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "isOn", "0");
        cJSON_AddStringToObject(json, "isDetected", "0");
        char *json_string = cJSON_Print(json);
        MqttPublisher::getInstance()->publishMessage("led", json_string);

        cJSON *json1 = cJSON_CreateObject();
        cJSON_AddStringToObject(json1, "isDetected", "0");
        char *json_string1 = cJSON_Print(json1);
        MqttPublisher::getInstance()->publishMessage("ultrasonic", json_string1);
    }
}
float UltraSonic::GetDistance_cm()
{
    digitalWrite(Trigger_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trigger_PIN, LOW);

    float duration = pulseIn(Echo_PIN, HIGH);

    return (0.034 * duration) / 2.0;
}
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
        char *json_string = cJSON_Print(json);
        MqttPublisher::getInstance()->publishMessage("led", json_string);
    }
    else
    {
        if (!Ison)
            return;

        Ison = false;
        digitalWrite(Led_Pin, LOW);
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "isOn", "0");
        char *json_string = cJSON_Print(json);
        MqttPublisher::getInstance()->publishMessage("led", json_string);
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
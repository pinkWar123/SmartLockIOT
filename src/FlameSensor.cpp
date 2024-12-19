#include "header/FlameSensor.h"
#include "header/MqttPublisher.h"

#include <cJSON.h>
int FlameSensor::Get_Flame_intensity()
{
    return analogRead(Analog_PIN);
}

bool FlameSensor::Is_Flame()
{
    if (IsDigital)
        return digitalRead(Digital_PIN) == LOW;

    return Get_Flame_intensity() > 700;
}

void FlameSensor::Detect_Flame()
{
    unsigned long currentTime = millis();
    if (Is_Flame() && currentTime - lastCheckTime >= 2000)
    {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "isFire", "1");
        char *json_string = cJSON_Print(json);
        MqttPublisher *client = MqttPublisher::getInstance();
        client->publishMessage("flame", json_string);
        if (servo->get_Is_locked() == true)
            servo->Unlock();
        delay(1000);
        buzzer->Sound(5000);
        delay(1000);
        lastCheckTime = currentTime;
        Update_State(true);

        // Send MQTT here
    }
    else if (!Is_Flame() && currentTime - lastCheckTime >= 2000)
    {
        if (lastState == true)
        {
            lastState = false;
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "isFire", "0");
            char *json_string = cJSON_Print(json);
            MqttPublisher *client = MqttPublisher::getInstance();
            client->publishMessage("flame", json_string);
        }
    }
}

void FlameSensor::Update_State(bool newState)
{
    lastState = newState;
}
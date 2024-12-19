#include "header/Buzzer.h"
#include "header/MqttPublisher.h"

#include <cJSON.h>

void Buzzer::Sound(int time)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "isOn", "1");
    char *json_string = cJSON_Print(json);
    MqttPublisher *client = MqttPublisher::getInstance();
    client->publishMessage("buzzer", json_string);
    digitalWrite(Buzzer_Pin, HIGH);
    delay(time + 100);
    digitalWrite(Buzzer_Pin, LOW);

    cJSON *json1 = cJSON_CreateObject();

    cJSON_AddStringToObject(json1, "isOn", "0");
    char *json_string1 = cJSON_Print(json1);
    client->publishMessage("buzzer", json_string1);
}
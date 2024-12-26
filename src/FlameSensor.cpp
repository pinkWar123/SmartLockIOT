#include "header/FlameSensor.h"
#include "header/MqttPublisher.h"
#include "Wifi.h"
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

        SendFlameMassage();
        if (servo->get_Is_locked() == true)
            servo->Unlock(true);
        delay(1000);
        buzzer->Sound(5000);
        delay(1000);
        lastCheckTime = currentTime;
        Update_State(true);
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

void FlameSensor::SendFlameMassage()
{
    
    WiFiClient client;
    pushsafer = new Pushsafer("lI8JrdMRXeObvw4W6gl3", client);
    

    char message[100];
    sprintf(message, "Fire detected! Be careful!");

    pushsafer->debug = true;

    struct PushSaferInput input;
    input.message = message;
    input.title = "URGENT: FIRE ALERT!";
    input.sound = "8";           // Loud sound
    input.vibration = "1";       // Vibration
    input.icon = "1";            // Icon for fire
    input.iconcolor = "#FF0000"; // Red color for danger
    input.priority = "1";        // High priority
    input.device = "a";          // Default device
    input.url = "https://www.pushsafer.com";
    input.urlTitle = "More Info";
    input.picture = "";
    input.picture2 = "";
    input.picture3 = "";
    input.time2live = "3600"; // The message lives for 1 hour
    input.retry = "2";        // Retry after 2 minutes
    input.expire = "86400";   // Expires after 1 day
    input.confirm = "1";      // Request confirmation
    input.answer = "I acknowledge the fire alert!";
    input.answeroptions = "Yes,No";
    input.answerforce = "Yes"; // Force user to answer

    // Send event and print the result
    String result = pushsafer->sendEvent(input);
    Serial.println(result);
    Serial.println("Sent PushSafer Message.");
}

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
        WiFiClient wifiClient;
        Pushsafer pushSafer("lI8JrdMRXeObvw4W6gl3", wifiClient);
        pushSafer.debug = true;

        char message[100];
        sprintf(message, "vibration detected! Be careful!");

        struct PushSaferInput input;
        input.message = message;
        input.title = "URGENT: VIBRATION ALERT!";
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
        input.answer = "I acknowledge the vibration alert!";
        input.answeroptions = "Yes,No";
        input.answerforce = "Yes"; // Force user to answer

        // Send event and print the result
        String result = pushSafer.sendEvent(input);
        Serial.println(result);
        Serial.println("Sent PushSafer Message.");

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
#include <PubSubClient.h>
#include <WiFi.h>
#include <cJSON.h>
#include "header/MqttPublisher.h"

MqttPublisher::MqttPublisher(PubSubClient &mqttClient)
    : client(mqttClient) {}
void MqttPublisher::initialize(PubSubClient &mqttClient)
{
    if (instance == nullptr)
    {
        instance = new MqttPublisher(mqttClient);
    }
}
MqttPublisher *MqttPublisher::getInstance()
{
    return instance;
}

// Publish message to the MQTT broker
void MqttPublisher::publishMessage(char *topic, char *json_string)
{
    // Publish the message
    if (client.publish(topic, json_string, true))
    {
        Serial.println("Message published");
    }
    else
    {
        Serial.println("Failed to publish message");
    }
    // Clean up
    free(json_string);
}

// Subscribe to an MQTT topic
void MqttPublisher::subscribeToTopic(const String &subTopic)
{
    if (client.subscribe(subTopic.c_str()))
    {
        Serial.print("Subscribed to topic: ");
        Serial.println(subTopic);
    }
    else
    {
        Serial.println("Subscription failed");
    }
}
// Initialize the static member
MqttPublisher *MqttPublisher::instance = nullptr;

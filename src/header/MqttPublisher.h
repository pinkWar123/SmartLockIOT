#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include <PubSubClient.h>
#include <WiFi.h>
#include <cJSON.h>

class MqttPublisher
{
private:
    static MqttPublisher *instance; // Static instance of the class
    PubSubClient &client;           // Reference to the MQTT client

    // Private constructor to prevent multiple instances
    MqttPublisher(PubSubClient &mqttClient);

public:
    // Public method to get the single instance of the class
    static void initialize(PubSubClient &mqttClient);
    static MqttPublisher *getInstance();

    // Publish message to the MQTT broker
    void publishMessage(char *topic, char *json_string);

    // Subscribe to an MQTT topic
    void subscribeToTopic(const String &subTopic);
};

#endif // MQTT_PUBLISHER_H

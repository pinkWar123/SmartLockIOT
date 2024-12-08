#include <Keypad.h>
#include <TM1637Display.h>
#include <math.h>
#include <WorkScheduler.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Timer.h"
#include "WiFi.h"
#include <PubSubClient.h>
#include "MQTTClient.h"
#include <cJSON.h>

// self library
////######################################
#include "header/ultraSonic.h"
#include "header/SerVo.h"
#include "header/KeyPad.h"
#include "header/Buzzer.h"
#include "header/MqttPublisher.h"
////######################################

////////////////////////////////////////////////////////// Network define
#define ADDRESS "mqtt://emqx@127.0.0.1:1883"
#define CLIENTID "mqttx_702058d9"
#define TOPIC "testtopic/1"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L
////////////////////////////////////////////////////////////////////////

// define pin

//////////////////////////////////////////////////////////////
// ultrasonic pins
#define TRIGGER_PIN 7
#define ECHO_PIN 6

// Servo Pins
#define SERVO_PIN 21
#define SERVO_LOCK_POS 0
#define SERVO_UNLOCK_POS 90

// keypad pins
byte rowPins[4] = {2, 0, 4, 16};
byte colPins[4] = {17, 5, 18, 19};

// s7-segment pins
const int CLK = 13;
const int DIO = 32;

// lcd i2c pins
const int SDA_PIN = 33;
const int SCL_PIN = 25;

// buzzer pins
const int buzzer_pin = 26;

// led pin
const int led_pin = 22;
////////////////////////////#############

// ############################
// declare device
UltraSonic *ultrasonic = new UltraSonic(TRIGGER_PIN, ECHO_PIN);
SerVo *servo = nullptr; 
TM1637Display *tm = new TM1637Display(CLK, DIO);
LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(0x27, 16, 2);
Buzzer *buzzer = new Buzzer(buzzer_pin);
KeyPad *keypad = nullptr; 

////######################################

// Keypad State

void task()
{
    Serial.print("RUN");
}

const char *ssid = "Wokwi-GUEST";
const char *password = "";

// MQTT broker
const char *mqtt_server = "172.16.1.229";
// Use your broker address
const int mqtt_port = 1883; // Default MQTT port
const char *mqttUser = "nguyenhongquan_thingsboard";
const char *mqttPassword = ""; // Empty since no password is set in MQTTX

WiFiClient espClient;
PubSubClient client(espClient);

// void lock()
// {
//     lockServo.write(SERVO_LOCK_POS);
//     cJSON *json = cJSON_CreateObject();
//     cJSON_AddStringToObject(json, "isOn", "false");
//     char *json_string = cJSON_Print(json);
//     client.publish("lock", json_string, true);
//     lcd.clear();
//     lcd.print("Locked!");
//     free(json_string);
//     cJSON_Delete(json);
// }

// void unlock()
// {
//     lockServo.write(SERVO_UNLOCK_POS);
//     // cJSON *json = cJSON_CreateObject();
//     // cJSON_AddStringToObject(json, "isOn", "true");
//     // char *json_string = cJSON_Print(json);
//     // client.publish("lock", json_string, true);
//     // lcd.clear();
//     // lcd.print("Unlocked!");
//     // free(json_string);
//     // cJSON_Delete(json);
// }v

void setup_wifi()
{
    delay(10);
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
        retries++;
        if (retries > 20)
        { // Timeout after 20 seconds
            Serial.println("Failed to connect to Wi-Fi!");
            return; // Exit if connection fails
        }
    }

    Serial.println("Connected to Wi-Fi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);

    Serial.print(". Message: ");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
    }
    Serial.println();
}

void connectToMQTT()
{
    while (!client.connected())
    {
        Serial.print("Connecting to MQTT...");
        if (client.connect("ESP32Client", mqttUser, mqttPassword))
        { // Provide username/password
            Serial.println("Connected!");
            client.subscribe("your/topic"); // Replace with your topic
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.println(client.state());
            delay(5000); // Retry every 5 seconds
        }
    }
}

// Initialize
void setup()
{
    Wire.begin(SDA_PIN, SCL_PIN);

    Serial.begin(115200);
    servo = new SerVo(SERVO_PIN, SERVO_LOCK_POS, SERVO_UNLOCK_POS);
    servo->Setup();
    keypad = new KeyPad(rowPins, colPins, tm, lcd,servo, buzzer);

    // Timer::getInstance()->initialize();

    // // Khởi tạo một công việc (job) - không đùng đến một pin cụ thể nào đó mà chỉ thực hiện các tác vụ như in serial monitor hoăc đọc các cảm biến có nhiều chân ^_^
    // setup_wifi();
    // // auto ip = WiFi.localIP();

    // client.setServer(mqtt_server, mqtt_port);
    // client.setCallback(callback);
    // connectToMQTT();
    // MqttPublisher::initialize(client);
    // xTaskCreate();

    ultrasonic->Setup();
    tm->setBrightness(0x0f);

    lcd->begin(16, 2);
    lcd->backlight();

    lcd->setCursor(0, 0);
}

// Handle user input

// Main Loop
bool isOn = false;
void loop()
{
    // if (!client.connected())
    // {
    //     connectToMQTT();
    // }
    // client.loop();
    // input();

    // Example: Publish a message every 5 seconds
    // static unsigned long lastPublish = 0;
    // if (millis() - lastPublish > 5000)
    // {
    //     lastPublish = millis();
    //     cJSON *json = cJSON_CreateObject();
    //     if (json == NULL)
    //     {
    //         fprintf(stderr, "Failed to create JSON object\n");
    //         return;
    //     }
    //     cJSON_AddStringToObject(json, "isOn", isOn ? "true" : "false");
    //     char *json_string = cJSON_Print(json);
    //     if (json_string == NULL)
    //     {
    //         fprintf(stderr, "Failed to print JSON string\n");
    //         cJSON_Delete(json);
    //         return;
    //     }
    //     client.publish("led", json_string, true);
    //     Serial.println("Message published");

    //     free(json_string);
    //     cJSON_Delete(json);
    //     isOn = !isOn;
    // }
    keypad->Input_key();
}

#include <LiquidCrystal.h>
#include <ESP32Servo.h>
#include <Keypad.h>
#include <TM1637Display.h>
#include <math.h>
#include <WorkScheduler.h>
#include "Timer.h"
#include "WiFi.h"
#include <PubSubClient.h>
#include "MQTTClient.h"
#include <cJSON.h>

#define ADDRESS "mqtt://emqx@127.0.0.1:1883"
#define CLIENTID "mqttx_702058d9"
#define TOPIC "testtopic/1"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L
// LCD Pins
int RS = 12;
int E = 14;
int D4 = 27;
int D5 = 26;
int D6 = 25;
int D7 = 33;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

// Shift Register Pins
#define DATA_PIN 2
#define CLOCK_PIN 0
#define LATCH_PIN 4

// Servo Pins
#define SERVO_PIN 21
#define SERVO_LOCK_POS 20
#define SERVO_UNLOCK_POS 90

#define TRIGGER_PIN 7
#define ECHO_PIN 6
Servo lockServo;

// Keypad Config
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {2, 0, 4, 16};
byte colPins[KEYPAD_COLS] = {17, 5, 18, 19};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
// Keypad State
char result[5] = "";
const char *KEY = "1234";
static int pos = 0; // Position on the LCD and in the result array

const int CLK = 13;
const int DIO = 32;

TM1637Display tm(CLK, DIO);

// Function to control the lock

WorkScheduler *inputAsync;
WorkScheduler *ultraSonicAsync;

uint8_t data[4];
int currentIndex = 0;

void ultrasonic()
{
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Read the result:
    int duration = pulseIn(ECHO_PIN, HIGH);
    Serial.print("Distance in CM: ");
    Serial.println(duration / 58);
    Serial.print("Distance in inches: ");
    Serial.println(duration / 148);
}

WorkScheduler *taskAsync;
void task()
{
    Serial.print("RUN");
}

const char *ssid = "Wokwi-GUEST";
const char *password = "";

// MQTT broker
const char *mqtt_server = "10.0.8.193";
// Use your broker address
const int mqtt_port = 1883; // Default MQTT port
const char *mqttUser = "nguyenhongquan_thingsboard";
const char *mqttPassword = ""; // Empty since no password is set in MQTTX

WiFiClient espClient;
PubSubClient client(espClient);

void lock()
{
    lockServo.write(SERVO_LOCK_POS);
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "isOn", "false");
    char *json_string = cJSON_Print(json);
    client.publish("lock", json_string, true);
    lcd.clear();
    lcd.print("Locked!");
    free(json_string);
    cJSON_Delete(json);
}

void unlock()
{
    lockServo.write(SERVO_UNLOCK_POS);
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "isOn", "true");
    char *json_string = cJSON_Print(json);
    client.publish("lock", json_string, true);
    lcd.clear();
    lcd.print("Unlocked!");
    free(json_string);
    cJSON_Delete(json);
}

void input()
{
    char key = keypad.getKey(); // Custom key scan logic

    if (key != '\0')
    {
        Serial.print("Key detected: ");
        Serial.println(key);

        if (key >= '0' && key <= '9')
        {
            lcd.print(key);
            result[pos++] = key; // Store the key
            result[pos] = '\0';  // Null-terminate the string
            if (currentIndex < 3)
            {
                data[currentIndex++] = tm.encodeDigit(key - '0');
            }
            else
            {
                data[currentIndex] = tm.encodeDigit(key - '0');
                currentIndex = 0;
                tm.clear();
            }

            if (pos == 4) // Check if 4 digits are entered
            {
                if (strcmp(result, KEY) == 0)
                {
                    unlock();
                }
                else
                {
                    lock();
                    lcd.clear();
                    lcd.print("Incorrect Code");
                }

                delay(1000);
                pos = 0;          // Reset position
                result[0] = '\0'; // Reset result array
                lcd.clear();
            }

            tm.setSegments(data, currentIndex, 0);
        }
    }
}

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
    tm.setBrightness(0x0f);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(22, OUTPUT);
    for (int col = 0; col < KEYPAD_COLS; col++)
    {
        pinMode(DATA_PIN + col, INPUT_PULLDOWN); // Ensure columns are stable
    }

    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    Serial.begin(115200);
    Serial.println("Hello, ESP32!");

    lcd.begin(16, 2);
    lcd.clear();

    lockServo.attach(SERVO_PIN);
    lock(); // Lock initially

    Timer::getInstance()->initialize();

    // Khởi tạo một công việc (job) - không đùng đến một pin cụ thể nào đó mà chỉ thực hiện các tác vụ như in serial monitor hoăc đọc các cảm biến có nhiều chân ^_^
    inputAsync = new WorkScheduler(0, input);

    // print ra nhanh hơn
    ultraSonicAsync = new WorkScheduler(200UL, ultrasonic);
    taskAsync = new WorkScheduler(100UL, task);
    setup_wifi();
    auto ip = WiFi.localIP();

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    xTaskCreate(

    );
}

// Handle user input

// Main Loop
bool isOn = false;
void loop()
{
    if (!client.connected())
    {
        connectToMQTT();
    }
    client.loop();
    input();

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
}

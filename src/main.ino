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
#include <freertos/FreeRTOS.h>
// self library
////######################################
#include "header/ultraSonic.h"
#include "header/SerVo.h"
#include "header/KeyPad.h"
#include "header/Buzzer.h"
#include "header/MqttPublisher.h"
#include "header/FlameSensor.h"
#include "header/Vibration_Sensor.h"
#include "header/RfidModule.h"
#include "header/Pushsafer.h"
////######################################

////////////////////////////////////////////////////////// Network define
#define ADDRESS "mqtt://emqx@127.0.0.1:1883"
#define CLIENTID "mqttx_702058d9"
#define TOPIC "testtopic/1"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L
#define PUSHSAFERKEY "lI8JrdMRXeObvw4W6gl3"
////////////////////////////////////////////////////////////////////////

// define pin

//////////////////////////////////////////////////////////////
// ultrasonic pins - done
#define TRIGGER_PIN 12
#define ECHO_PIN 3

// Servo Pins - done
#define SERVO_PIN 13
#define SERVO_LOCK_POS 0
#define SERVO_UNLOCK_POS 90

// keypad pins - done
byte colPins[4] = {27, 26, 25, 33};
byte rowPins[4] = {32, 17, 16, 2};

// s7-segment pins - done
const int CLK = 0;
const int DIO = 14;

// lcd i2c pins - done
const int SDA_PIN = 21;
const int SCL_PIN = 22;

// buzzer pins - done
#define buzzer_pin 4

// led pin
const int led_pin = 1;

// flame sensor - done
const int flame_pin = 34;

// vibrant sensor
const int vibrant_pin = 35;

// RFID - done
const int SS_PIN = 5;
const int MOSI_PIN = 23;
const int MISO_PIN = 19;
const int SCK_PIN = 18;
const int RST_PIN = 15;

////////////////////////////#############

WiFiClient espClient;
PubSubClient client(espClient);
Pushsafer *pushsafer = nullptr;
// ############################
// declare device
UltraSonic *ultrasonic = new UltraSonic(TRIGGER_PIN, ECHO_PIN, led_pin);
SerVo *servo = new SerVo(SERVO_PIN, SERVO_LOCK_POS, SERVO_UNLOCK_POS);
TM1637Display *tm = new TM1637Display(CLK, DIO);
LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(0x27, 16, 2);
Buzzer *buzzer = new Buzzer(buzzer_pin);
KeyPad *keypad = nullptr;

FlameSensor *flamesensor = new FlameSensor(flame_pin, servo, buzzer, pushsafer);
VibrationSensor *vibrationSensor = new VibrationSensor(vibrant_pin, buzzer, servo, true);
RfidModule *rfid = new RfidModule(SS_PIN, RST_PIN, servo, buzzer, lcd);
////######################################

// Keypad State

void task()
{
    Serial.print("RUN");
}

// const char *ssid = "Wokwi-GUEST";
const char *ssid = "Buồng ngũ quá";
const char *password = "nguyenkhanh0209";

// MQTT broker
const char *mqtt_server = "192.168.48.144";
// Use your broker address
const int mqtt_port = 1883; // Default MQTT port
const char *mqttUser = "nguyenhongquan_thingsboard";
const char *mqttPassword = ""; // Empty since no password is set in MQTTX



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

    if (strcmp(topic, "lock") == 0)
    {
        Serial.println("Processing lock state...");

        // Convert the message payload to a string
        String payload = "";
        for (int i = 0; i < length; i++)
        {
            payload += (char)message[i];
        }

        Serial.print("Payload: ");
        Serial.println(payload);

        // Parse JSON payload
        StaticJsonDocument<200> doc; // Adjust size as needed
        DeserializationError error = deserializeJson(doc, payload);

        if (error)
        {
            Serial.print("JSON Parsing failed: ");
            Serial.println(error.c_str());
            return;
        }
        const char *isLocked = doc["isLocked"];

        // Extract "isLocked" value
        if (strcmp(isLocked, "1") == 0)
        {
            servo->Lock(false); // Call Lock() when isLocked is "1"
        }
        else if (strcmp(isLocked, "0") == 0)
        {
            servo->Unlock(false); // Call Unlock() when isLocked is "0"
        }
        else
        {
            Serial.println("Invalid value for isLocked. Ignoring...");
        }
    }

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

void Task_Keypad(void *pvParameters)
{
    // Logic để xử lý bàn phím
    while (true)
    {
        keypad->Input_key();
        delay(100); // Điều chỉnh thời gian delay nếu cần
    }
}

void Task_FlameSensor(void *pvParameters)
{
    // Logic để xử lý cảm biến ngọn lửa
    while (true)
    {
        flamesensor->Detect_Flame();
        delay(100); // Điều chỉnh thời gian delay nếu cần
    }
}

void Task_VibrationSensor(void *pvParameters)
{
    // Logic để xử lý cảm biến rung
    while (true)
    {
        vibrationSensor->Detect_Vibrant();
        delay(100); // Điều chỉnh thời gian delay nếu cần
    }
}

void Task_RFID(void *pvParameters)
{
    // Logic để xử lý RFID
    while (true)
    {
        rfid->Detect_Card();
        delay(100); // Điều chỉnh thời gian delay nếu cần
    }
}

void Task_Ultrasonic(void *pvParameters)
{
    // Logic để xử lý cảm biến siêu âm
    while (true)
    {
        ultrasonic->Detect_object();
        delay(100); // Điều chỉnh thời gian delay nếu cần
    }
}

void setup()
{
    Serial.begin(9600);
    Wire.begin(SDA_PIN, SCL_PIN);
    servo->Lock(false);

    keypad = new KeyPad(rowPins, colPins, tm, lcd, servo, buzzer);

    // Timer::getInstance()->initialize();

    // // Khởi tạo một công việc (job) - không đùng đến một pin cụ thể nào đó mà chỉ thực hiện các tác vụ như in serial monitor hoăc đọc các cảm biến có nhiều chân 😊
    setup_wifi();
    // // auto ip = WiFi.localIP();

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connectToMQTT();
    MqttPublisher::initialize(client);
    MqttPublisher::getInstance()->subscribeToTopic("lock");
    // xTaskCreate();

    ultrasonic->Setup();
    tm->setBrightness(7, true);
    tm->clear();

    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);

    lcd->init();
    lcd->begin(16, 2);
    lcd->backlight();

    lcd->setCursor(0, 0);
    rfid->Setup();
    rfid->Setup_keypad(keypad);
    lcd->print("hello world");
    pushsafer = new Pushsafer(PUSHSAFERKEY, espClient);


    // char message[100];
    // sprintf(message, "Fire detected! Be careful!");

    // pushsafer->debug = true;

    // struct PushSaferInput input;
    // input.message = message;
    // input.title = "URGENT: FIRE ALERT!";
    // input.sound = "8";  // Loud sound
    // input.vibration = "1";  // Vibration
    // input.icon = "1";  // Icon for fire
    // input.iconcolor = "#FF0000";  // Red color for danger
    // input.priority = "1";  // High priority
    // input.device = "a";  // Default device
    // input.url = "https://www.pushsafer.com";
    // input.urlTitle = "More Info";
    // input.picture = "";
    // input.picture2 = "";
    // input.picture3 = "";
    // input.time2live = "3600";  // The message lives for 1 hour
    // input.retry = "2";  // Retry after 2 minutes
    // input.expire = "86400";  // Expires after 1 day
    // input.confirm = "1";  // Request confirmation
    // input.answer = "I acknowledge the fire alert!";
    // input.answeroptions = "Yes,No";
    // input.answerforce = "Yes";  // Force user to answer

    // // Send event and print the result
    // String result = pushsafer->sendEvent(input);


    xTaskCreate(Task_Keypad, "Task_Keypad", 2048, NULL, 1, NULL);
    xTaskCreate(Task_FlameSensor, "Task_FlameSensor", 10000, NULL, 1, NULL);
    xTaskCreate(Task_VibrationSensor, "Task_VibrationSensor", 10000,NULL, 1, NULL);
    xTaskCreate(Task_Ultrasonic, "Task_Ultrasonic", 2048, NULL, 1, NULL);
    xTaskCreate(Task_RFID, "Task_RFID", 2048, NULL, 1, NULL);
}

// int servopos = 0;
// bool islock = true;
// int count = 0;
void loop()
{
    // Serial.print("");
    if (!client.connected())
    {
        connectToMQTT();
    }
    client.loop();
    // // input();

    // // Example: Publish a message every 5 seconds
    // // static unsigned long lastPublish = 0;
    // // if (millis() - lastPublish > 5000)
    // // {
    // //     lastPublish = millis();
    // //     cJSON *json = cJSON_CreateObject();
    // //     if (json == NULL)
    // //     {
    // //         fprintf(stderr, "Failed to create JSON object\n");
    // //         return;
    // //     }
    // //     cJSON_AddStringToObject(json, "isOn", isOn ? "true" : "false");
    // //     char *json_string = cJSON_Print(json);
    // //     if (json_string == NULL)
    // //     {
    // //         fprintf(stderr, "Failed to print JSON string\n");
    // //         cJSON_Delete(json);
    // //         return;
    // //     }
    // //     client.publish("led", json_string, true);
    // //     Serial.println("Message published");

    // //     free(json_string);
    // //     cJSON_Delete(json);
    // //     isOn = !isOn;
    // // }
    // // buzzer->Sound(300);
    // // delay(1000);
    // keypad->Input_key();
    // flamesensor->Detect_Flame();
    // vibrationSensor->Detect_Vibrant();
    // rfid->Detect_Card();
    // ultrasonic->Detect_object();

    // // lcd->setCursor(0, 0); // Đặt con trỏ ở dòng đầu tiên
    // // lcd->print("Hello world"); // In chuỗi lên màn hình
    // // delay(5000); // Chờ 5 giây để nội dung hiển thị
    // // lcd->clear(); // Xóa màn hình
    // // delay(1000); // Chờ 1 giây trước khi lặp lại

    // // for (int minutes = 0; minutes < 60; minutes++) {
    // //     for (int seconds = 0; seconds < 60; seconds++) {
    // //         int time = minutes * 100 + seconds; // Định dạng MM:SS
    // //         tm->showNumberDecEx(time, 0b01000000, true); // Hiển thị dấu ':'
    // //         delay(1000); // Dừng 1 giây
    // //     }
    // // }
}
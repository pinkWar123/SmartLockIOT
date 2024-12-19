#include "header/KeyPad.h"
#include "header/MqttPublisher.h"

#include <cJSON.h>
char key_num[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
KeyPad::KeyPad(byte rowPins[], byte colPins[], TM1637Display *tm, LiquidCrystal_I2C *lcd, SerVo *servo, Buzzer *buzzer)
    : keypad(makeKeymap(key_num), rowPins, colPins, 4, 4), tm(tm), lcd(lcd), servo(servo), buzzer(buzzer)
{
    for (int i = 0; i < 4; i++)
    {
        this->rowPins[i] = rowPins[i];
        this->colPins[i] = colPins[i];
    }
}

void KeyPad::Input_key()
{
    char key = keypad.getKey(); // Custom key scan logic

    if (key != '\0')
    {
        Serial.print("Key detected: ");
        Serial.println(key);

        if (key >= '0' && key <= '9')
        {
            if (pos == 0)
                lcd->clear();

            lcd->print(key);
            result[pos++] = key; // Store the key
            result[pos] = '\0';  // Null-terminate the string
            if (currentIndex < 3)
            {
                data[currentIndex++] = tm->encodeDigit(key - '0');
            }
            else
            {
                data[currentIndex] = tm->encodeDigit(key - '0');
                currentIndex = 0;
                tm->clear();
            }

            if (pos == 4) // Check if 4 digits are entered
            {
                // cJSON *json = cJSON_CreateObject();
                // if (json == NULL)
                // {
                //     fprintf(stderr, "Failed to create JSON object\n");
                //     return;
                // }

                Serial.println(result);

                if (strcmp(result, KEY) == 0)
                {
                    // cJSON_AddStringToObject(json, "isOn", "false");
                    lcd->clear();
                    lcd->print("Correct Code");
                    servo->Unlock();
                    delay(500);
                    buzzer->Sound(500);
                    delay(1000);
                    CountInvalidRegisters = 0;

                    // Send MQTT request here
                    // cJSON *json = cJSON_CreateObject();
                    // cJSON_AddStringToObject(json, "isLocked", "0");
                    // char *json_string = cJSON_Print(json);
                    // MqttPublisher *client = MqttPublisher::getInstance();
                    // client->publishMessage("lock", json_string);
                }
                else
                {
                    // cJSON_AddStringToObject(json, "isOn", "true");
                    lcd->clear();
                    lcd->print("InCorrect Code");
                    servo->Lock();
                    delay(500);
                    buzzer->Sound(500);
                    delay(1000);
                    CountInvalidRegisters++;
                    Serial.println("Number of Invalid Registers : " + CountInvalidRegisters);
                    if (CountInvalidRegisters == 5)
                    {
                        buzzer->Sound(5000);
                        delay(5500);

                        // Send MQTT Request Here
                    }
                }

                // MqttPublisher *client = MqttPublisher::getInstance();
                // char *json_string = cJSON_Print(json);
                // if (json_string == NULL)
                // {
                //     fprintf(stderr, "Failed to print JSON string\n");
                //     cJSON_Delete(json);
                //     return;
                // }

                // client->publishMessage("lock", json_string);
                // cJSON_Delete(json);

                // Wait 1 second
                delay(1000);
                pos = 0;          // Reset position
                result[0] = '\0'; // Reset result array
                lcd->clear();
            }

            tm->setSegments(data, currentIndex, 0);
        }
    }
}

void KeyPad::Reset_data()
{
    // tm->clear();
    pos = 0;
    result[0] = '\0';
    lcd->clear();
    currentIndex = 0;
}
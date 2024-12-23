#include "header/RfidModule.h"
#include "header/MqttPublisher.h"

#include <cJSON.h>

char *convertUidToString(uint8_t *uidByte, size_t size)
{
    char *uidString = (char *)malloc(size * 2 + 1); // Each byte = 2 hex chars + 1 for null terminator
    if (uidString == NULL)
    {
        return NULL; // Handle allocation failure
    }

    for (size_t i = 0; i < size; i++)
    {
        sprintf(&uidString[i * 2], "%02X", uidByte[i]); // Convert byte to hex and append
    }
    uidString[size * 2] = '\0'; // Null-terminate the string
    return uidString;           // Caller must free the allocated memory
}

void RfidModule::Detect_Card()
{
    if (rfid->PICC_IsNewCardPresent() && rfid->PICC_ReadCardSerial())
    {
        buzzer->Sound(200);
        delay(200);
        Serial.print("UID: ");
        keypad->Reset_data();
        for (byte i = 0; i < rfid->uid.size; i++)
        {
            Serial.print(rfid->uid.uidByte[i] < 0x10 ? " 0" : " "); // In định dạng Hex
            Serial.print(rfid->uid.uidByte[i], HEX);
        }
        Serial.println();
        char *uidStr = convertUidToString(rfid->uid.uidByte, rfid->uid.size);
        if (isCardValid(rfid->uid.uidByte, rfid->uid.size))
        {
            Serial.println("valid card");
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "status", "1");
            cJSON_AddStringToObject(json, "uid", uidStr);
            char *json_string = cJSON_Print(json);
            MqttPublisher *client = MqttPublisher::getInstance();
            client->publishMessage("rfid", json_string, false);
            servo->Unlock(true);
            delay(1000);

            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->printf("Valid card");
            lcd->setCursor(0, 7);
            lcd->printf("Welcome");

            delay(3500);
            lcd->clear();
            // send MQTT here
        }
        else
        {
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "status", "0");
            cJSON_AddStringToObject(json, "uid", uidStr);
            char *json_string = cJSON_Print(json);
            MqttPublisher *client = MqttPublisher::getInstance();
            client->publishMessage("rfid", json_string, false);
            Serial.println("invalid card");
            lcd->setCursor(0, 0);
            lcd->printf("inValid card");
            servo->Lock(true);
        }

        rfid->PICC_HaltA();      // Dừng giao tiếp với thẻ hiện tại
        rfid->PCD_StopCrypto1(); // Ngừng mã hóa
        return;
    }

    if (!rfid->PICC_IsNewCardPresent())
    {
        delay(500);
    }
    else if (!rfid->PICC_ReadCardSerial())
    {
        Serial.println("Failed to read card.");
        rfid->PCD_Reset();
        delay(500);
    }
}

bool RfidModule::isCardValid(byte *uid, byte uidSize)
{
    for (int i = 0; i < numberOfCard; i++)
    {
        bool match = true;
        for (int j = 0; j < uidSize; j++)
        {
            if (validCards[i][j] != uid[j])
            {
                match = false;
                break;
            }
        }
        if (match)
            return true;
    }
    return false;
}
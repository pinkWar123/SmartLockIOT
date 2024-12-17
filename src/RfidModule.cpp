#include "header/RfidModule.h"

void RfidModule::Detect_Card()
{
    if (rfid->PICC_IsNewCardPresent() && rfid->PICC_ReadCardSerial())
    {
        Serial.print("UID: ");
        keypad->Reset_data();
        for (byte i = 0; i < rfid->uid.size; i++)
        {
            Serial.print(rfid->uid.uidByte[i] < 0x10 ? " 0" : " "); // In định dạng Hex
            Serial.print(rfid->uid.uidByte[i], HEX);
        }
        Serial.println();
        if (isCardValid(rfid->uid.uidByte, rfid->uid.size))
        {
            Serial.println("valid card");
            lcd->clear();
            lcd->setCursor(0,0);
            lcd->printf("Valid card");
            lcd->setCursor(0,7);
            lcd->printf("Welcome");
            servo->Unlock();

            // send MQTT here
        }
        else
        {
            Serial.println("invalid card");
            lcd->setCursor(0,0);
            lcd->printf("inValid card");
            servo->Lock();
        }

        rfid->PICC_HaltA();      // Dừng giao tiếp với thẻ hiện tại
        rfid->PCD_StopCrypto1(); // Ngừng mã hóa
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
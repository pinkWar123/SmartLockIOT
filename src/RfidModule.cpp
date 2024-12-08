#include "header/RfidModule.h"

void RfidModule::Setup()
{
    SPI.begin();
    rfid.PCD_Init();

    Serial.print(F("Reader :"));
    rfid.PCD_DumpVersionToSerial();
}

void RfidModule::ReadRFID()
{
    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    if (!rfid.PICC_IsNewCardPresent())
        return;


    if (!rfid.PICC_ReadCardSerial())
        return;

    for (byte i = 0; i < 4; i++)
    {
        nuidPICC[i] = rfid.uid.uidByte[i];
    }

    Serial.print(F("RFID In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    rfid.PICC_HaltA();

    rfid.PCD_StopCrypto1();
}

void RfidModule::printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void RfidModule::printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}
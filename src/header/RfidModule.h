#pragma once
#include "Device.h"
#include "SerVo.h"
#include "KeyPad.h"
#include "Buzzer.h"

class RfidModule : public Device
{
private:
    int Ss_PIN;
    int Rst_PIN;

    LiquidCrystal_I2C *lcd = nullptr;
    SerVo *servo = nullptr;
    KeyPad *keypad = nullptr;

    MFRC522 *rfid = nullptr;

    int numberOfCard = 2;
    byte validCards[2][4] = {
        {0xA3, 0x8E, 0xE7, 0xF7},
        {0xE3, 0xEC, 0x62, 0x1A}};

    bool isCardValid(byte *uid, byte uidSize);

public:
    RfidModule(int Ss_PIN, int Rst_PIN, SerVo *servo, LiquidCrystal_I2C *lcd)
        : Ss_PIN(Ss_PIN), Rst_PIN(Rst_PIN), servo(servo), lcd(lcd)
    {
        rfid = new MFRC522(Ss_PIN, Rst_PIN);
    }

    void Setup() override
    {
        SPI.begin(); // init SPI bus
        rfid->PCD_Init();                               // init MFRC522
    }

    void Setup_keypad(KeyPad *keypad)
    {
        this->keypad = keypad;
    }

    void Detect_Card();

    ~RfidModule() {}
};

#pragma once
#include "Device.h"
#include "SerVo.h"

class RfidModule : public Device
{
private:
    int Ss_PIN;
    int Rst_PIN;

    LiquidCrystal_I2C *lcd = NULL;
    SerVo *servo = NULL;

    MFRC522::MIFARE_Key key;
    MFRC522 rfid;

    static constexpr int IpAddress[4] = {103, 97, 67, 25};
    byte nuidPICC[4] = {0, 0, 0, 0};

    void printHex(byte *buffer, byte bufferSize);
    void printDec(byte *buffer, byte bufferSize);

public:
    RfidModule() : rfid(10, 9), Ss_PIN(10), Rst_PIN(9), key(), nuidPICC{0, 0, 0, 0} {}

    RfidModule(int Ss_PIN, int Rst_PIN) : rfid(Ss_PIN, Rst_PIN), Ss_PIN(Ss_PIN), Rst_PIN(Rst_PIN), key(), nuidPICC{0, 0, 0, 0} {}

    void Setup_lcd(LiquidCrystal_I2C *lcd) {
        this->lcd = lcd;
    }

    void Setup_SerVo(SerVo *servo) {
        this->servo = servo;
    }

    void Setup() override;

    void ReadRFID();

    ~RfidModule() {}
};

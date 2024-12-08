#pragma once
#include "Device.h"
#include "SerVo.h"
#include "Buzzer.h"

extern char key_num[4][4];

class KeyPad : public Device
{
private:
    byte rowPins[4];
    byte colPins[4];
    Keypad keypad;

    TM1637Display *tm;
    LiquidCrystal_I2C *lcd;
    SerVo *servo;
    Buzzer *buzzer;

    char result[5] = "";
    const char *KEY = "1234";
    int pos = 0;

    uint8_t data[4];
    int currentIndex = 0;

public:
    KeyPad(byte rowPins[], byte colPins[], TM1637Display *tm, LiquidCrystal_I2C *lcd, SerVo *&servo, Buzzer *buzzer);

    void Setup() override {};
    void Input_key();
    ~KeyPad() {}
};

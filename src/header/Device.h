#pragma once
#include <Keypad.h>
#include <TM1637Display.h>
#include <ESP32Servo.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

class Device
{
    private:
    public:
    Device(){}
    virtual void Setup(){}
    virtual ~Device(){}
};
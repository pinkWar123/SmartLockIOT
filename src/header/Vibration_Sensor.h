#pragma once
#include "Device.h"
#include "Buzzer.h"
#include "SerVo.h"

class VibrationSensor : public Device
{
private:
    int Digital_PIN;
    int Analog_Pin;

    Buzzer *buzzer = nullptr;
    SerVo *servo = nullptr;

    bool lastState = false;

    bool is_Digital = true;

    bool Is_Vibrant();
    float Get_Vibrant_Intensity();

    unsigned long lastCheckTime = 0;

public:
    VibrationSensor() {}

    VibrationSensor(int pin, Buzzer *buzzer, SerVo *servo, bool is_digital = true)
    {
        this->buzzer = buzzer;
        this->servo = servo;
        this->is_Digital = is_digital;
        is_Digital ? Digital_PIN = pin : Analog_Pin = pin;
        pinMode(pin, INPUT);
    }

    void Detect_Vibrant();

    ~VibrationSensor() = default;
};
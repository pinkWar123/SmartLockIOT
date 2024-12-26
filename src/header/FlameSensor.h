#pragma once
#include "Device.h"
#include "SerVo.h"
#include "Buzzer.h"
#include "Pushsafer.h"
#include "WiFi.h"

class FlameSensor : public Device
{
private:
    int Digital_PIN = 0; 
    int Analog_PIN = 0;

    bool IsDigital;

    SerVo *servo = nullptr;
    Buzzer *buzzer = nullptr;
    Pushsafer *pushsafer = nullptr;

    unsigned long lastCheckTime = 0;
    bool lastState = false;

    bool Is_Flame();
    int Get_Flame_intensity();
    void SendFlameMassage();

public:
    FlameSensor() {}

    FlameSensor(int pin, SerVo *servo, Buzzer *buzzer, Pushsafer *pushsafer ,bool IsDigital = true)
    {
        this->IsDigital = IsDigital;
        this->servo = servo;
        this->buzzer = buzzer;

        IsDigital ? Digital_PIN = pin : Analog_PIN = pin;
        pinMode(pin, INPUT);
    }

    void Detect_Flame();
    void Update_State(bool);

    ~FlameSensor() = default;
};
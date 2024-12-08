#pragma once
#include "Device.h"



class FlameSensor : public Device
{
    private: 
        int Digital_PIN;
        int Analog_PIN;

        bool IsDigital;

    public:
        FlameSensor(){}

        FlameSensor(int pin, bool IsDigital = true)
        {
            this->IsDigital = IsDigital;
            IsDigital ? Digital_PIN = pin :Analog_PIN = pin;
        }

        bool Is_Flame();
        int Get_Flame_intensity();

        ~FlameSensor();
};
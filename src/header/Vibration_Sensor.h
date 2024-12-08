#pragma once
#include "Device.h"



class VibrationSensor : public Device
{
    private: 
        int Digital_PIN;

    public:
        VibrationSensor(){}

        VibrationSensor(int Digital_PIN) : Digital_PIN(Digital_PIN){}

        bool Is_Vibrant();

        ~VibrationSensor();
};
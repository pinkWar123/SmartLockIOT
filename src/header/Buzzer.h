#pragma once
#include "Device.h"


class Buzzer : public Device
{
    private:
        int Buzzer_Pin;
    
    public:
        Buzzer(){}
        Buzzer(int Buzzer_Pin) : Buzzer_Pin(Buzzer_Pin){}

        void Sound();
        ~Buzzer(){}
};
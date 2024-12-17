#pragma once
#include "Device.h"


class Buzzer : public Device
{
    private:
        int Buzzer_Pin;
        bool isActive = false;
    
    public:
        Buzzer(){}
        Buzzer(int Buzzer_Pin) : Buzzer_Pin(Buzzer_Pin){
            isActive = false;
            pinMode(Buzzer_Pin, OUTPUT);
        }

        void Sound(int time);

        bool GetActive() { return isActive; }
        void SetActive(bool value) { isActive = value; }
        ~Buzzer(){}

};
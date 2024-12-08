#include "header/Buzzer.h"


void Buzzer::Sound(int time)
{
    tone(Buzzer_Pin, 100, time);
}
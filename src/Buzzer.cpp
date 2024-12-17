#include "header/Buzzer.h"


void Buzzer::Sound(int time)
{
    tone(Buzzer_Pin, 1000);
    delay(time + 100);
    noTone(Buzzer_Pin);
}
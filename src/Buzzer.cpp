#include "header/Buzzer.h"


void Buzzer::Sound(int time)
{
    digitalWrite(Buzzer_Pin, HIGH);
    delay(time + 100);
    digitalWrite(Buzzer_Pin, LOW);
}
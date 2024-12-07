#include "header/Buzzer.h"


void Buzzer::Sound()
{
    tone(Buzzer_Pin, 100, 1500);
}
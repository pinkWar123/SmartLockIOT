#include "header/KeyPad.h"

char key_num[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
KeyPad::KeyPad(byte rowPins[], byte colPins[], TM1637Display *tm, LiquidCrystal_I2C *lcd, SerVo *servo, Buzzer *buzzer)
    : keypad(makeKeymap(key_num), rowPins, colPins, 4, 4), tm(tm), lcd(lcd), servo(servo) , buzzer(buzzer){
    for (int i = 0; i < 4; i++) {
        this->rowPins[i] = rowPins[i];
        this->colPins[i] = colPins[i];
    }
}

void KeyPad::Input_key()
{
    char key = keypad.getKey(); // Custom key scan logic

    if (key != '\0')
    {
        Serial.print("Key detected: ");
        Serial.println(key);

        if (key >= '0' && key <= '9')
        {
            lcd->print(key);
            result[pos++] = key; // Store the key
            result[pos] = '\0';  // Null-terminate the string
            if (currentIndex < 3)
            {
                data[currentIndex++] = tm->encodeDigit(key - '0');
            }
            else
            {
                data[currentIndex] = tm->encodeDigit(key - '0');
                currentIndex = 0;
                tm->clear();
            }

            if (pos == 4) // Check if 4 digits are entered
            {
                if (strcmp(result, KEY) == 0)
                {
                    servo->Unlock();
                    buzzer->Sound();
                }
                else
                {
                    servo->Lock();
                    lcd->clear();
                    buzzer->Sound();
                    lcd->print("Incorrect Code");
                }

                delay(1000);
                pos = 0;          // Reset position
                result[0] = '\0'; // Reset result array
                lcd->clear();
            }

            tm->setSegments(data, currentIndex, 0);
        }
    }
}
#include <LiquidCrystal.h>
#include <ESP32Servo.h>

// LCD Pins
int RS = 12;
int E = 14;
int D4 = 27;
int D5 = 26;
int D6 = 25;
int D7 = 33;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

// Shift Register Pins
#define DATA_PIN 2
#define CLOCK_PIN 0
#define LATCH_PIN 4

// Servo Pins
#define SERVO_PIN 21
#define SERVO_LOCK_POS 20
#define SERVO_UNLOCK_POS 90
Servo lockServo;

// Keypad Config
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Keypad State
char result[5] = "";
const char *KEY = "1234";
static int pos = 0; // Position on the LCD and in the result array

// Function to control the lock
void lock()
{
    lockServo.write(SERVO_LOCK_POS);
}

void unlock()
{
    lockServo.write(SERVO_UNLOCK_POS);
}

// Initialize
void setup()
{
    for (int col = 0; col < KEYPAD_COLS; col++)
    {
        pinMode(DATA_PIN + col, INPUT_PULLDOWN); // Ensure columns are stable
    }

    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    Serial.begin(115200);
    Serial.println("Hello, ESP32!");

    lcd.begin(16, 2);
    lcd.clear();

    lockServo.attach(SERVO_PIN);
    lock(); // Lock initially
}

// Function to send data via shift register
void shiftRegisterWrite(byte data)
{
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data); // Send data to shift register
    digitalWrite(LATCH_PIN, HIGH);
    delayMicroseconds(10);
}

// Scan keypad rows and columns
char getKey()
{
    for (int row = 0; row < KEYPAD_ROWS; row++)
    {
        byte rowData = ~(1 << row);  // Activate one row at a time
        shiftRegisterWrite(rowData); // Send row data to shift register

        for (int col = 0; col < KEYPAD_COLS; col++)
        {
            if (digitalRead(DATA_PIN + col) == HIGH) // Read column state
            {
                return keys[row][col];
            }
        }
    }
    return '\0'; // No key pressed
}

// Handle user input
void input()
{
    char key = getKey(); // Custom key scan logic

    if (key != '\0')
    {
        Serial.print("Key detected: ");
        Serial.println(key);

        if (key >= '0' && key <= '9')
        {
            lcd.print(key);
            result[pos++] = key; // Store the key
            result[pos] = '\0';  // Null-terminate the string

            if (pos == 4) // Check if 4 digits are entered
            {
                if (strcmp(result, KEY) == 0)
                {
                    unlock();
                    lcd.clear();
                    lcd.print("Unlocked!");
                }
                else
                {
                    lock();
                    lcd.clear();
                    lcd.print("Incorrect Code");
                }

                delay(1000);
                pos = 0;          // Reset position
                result[0] = '\0'; // Reset result array
                lcd.clear();
            }
        }
    }
}

// Main Loop
void loop()
{
    input();
    delay(50); // Add delay to debounce keys
}

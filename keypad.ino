#include <Keypad.h>

// Define the keypad matrix (4x4)
const byte ROW_NUM = 4; //Four rows
const byte COL_NUM = 4; //Four columns

char keys[ROW_NUM][COL_NUM] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'},
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //Connect to the row pinouts of the keypad
byte pin_column[COL_NUM] = {5, 4, 3, 2}; //Connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COL_NUM );

int timerDuration = 0; //Timer duration in seconds
unsigned long startTime = 0;
bool timerStarted = false;

const int buzzerPin = 10;
const int ledPin = 11;

void setup() {
    // Initialize buzzer and LED pins
    pinMode(buzzerPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    
    // Start the serial monitor for debugging
    Serial.begin(9600);
    
    //Print instructions on serial monitor
    Serial.println("Enter timer duration in seconds (3-digit max) : ");
}

void loop() {
    char key = keypad.getKey();
    
    if (key) {
        Serial.print("You pressed: ");
        Serial.println(key);
        
        //Handle key input for setting timer
        if (key >= '0' && key <= '9') {
            timerDuration = timerDuration * 10 + (key - '0'); //Build timer value by pressing digits
            Serial.print("Timer Duration: ");
            Serial.println(timerDuration);
        }
        
        //Start the timer
        if (key == '#') {
            if (timerDuration > 0) {
                startTime = millis();
                timerStarted = true;
                Serial.print("Timer Started: ");
                Serial.println(timerDuration);
            }   else {
                Serial.println("Please enter a valid time: ");
            }
        }
        
        // Reset the timer
        if (key == '*') {
            timerDuration = 0;
            timerStarted = false;
            Serial.println("Timer reset!");
        }
    }
    
    //Check if the timer is running
    if (timerStarted) {
        unsigned long elapsedTime = (millis() - startTime) / 1000; //Convert milliseconds to seconds
        if (elapsedTime >= timerDuration) {
            //Time is up! Trigger the alarm
            digitalWrite(buzzerPin, HIGH); //Turn on buzzer
            digitalWrite(ledPin, HIGH); //Turn on LED
            delay(1000);                //Keep the buzzer and LED
            digitalWrite(buzzerPin, LOW); //Turn off buzzer
            digitalWrite(ledPin, LOW);    //Turn off LED
            timerStarted = false;       //Reset the timer state
            Serial.println("Time's up!");
        }
    }
}
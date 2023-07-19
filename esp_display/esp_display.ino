#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

SoftwareSerial SensorSerial(D3, D4);  // rx, tx

unsigned long previousMillis = 0;
int distance;

void setup() {

  Serial.begin(115200);
  SensorSerial.begin(9600);

  lcd.begin(16, 2);  // initialize the lcd

}


void loop() {
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 2000) {
    previousMillis = currentMillis;
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print(distance);
  }

  while (SensorSerial.available() > 0) {
     distance =  SensorSerial.parseInt();

    if (SensorSerial.read() == '\n') {

    }
  }
}

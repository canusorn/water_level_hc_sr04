/*
การต่อสาย

จอ LCD -> ESP8266
SCL  ->  D1
SDA  ->  D2
VCC  ->  5v
GND  ->  GND

led -> ESP8266
bluePin    ->  D4  
greenPin   ->  D5
yellowPin  ->  D6
redPin     ->  D7

สายระหว่างบอร์ด -> ESP8266
แดง  ->  V1
ดำ   ->  GND
ฟ้า   ->  D3

*/



// level in centimeter [cm]
#define tankLevel 400    // ระดับน้ำสูงสุด [cm]
#define blueLevel 350    // ระดับน้ำแสดงไฟสีฟ้า [cm]
#define greenLevel 300   // ระดับน้ำแสดงไฟสีเขียว [cm]
#define yellowLevel 200  // ระดับน้ำแสดงไฟสีเหลือง [cm]
#define redLevel 100     // ระดับน้ำแสดงไฟสีแดง [cm]

// ขา led
#define bluePin D4  
#define greenPin D5
#define yellowPin D6
#define redPin D7

#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>   // ติดตั้งโดยค้นหา LiquidCrystal_PCF8574 จาก library manager ได้เลย
#include <TridentTD_LineNotify.h>    // ติดตั้งโดยค้นหา TridentTD_LineNotify จาก library manager ได้เลย

#define SSID        "ssid"       // ชื่อไวไฟ
#define PASSWORD    "pass"       // รหัสผ่าน
#define LINE_TOKEN  "linetoken"  // โทเค็นไวไฟ

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

SoftwareSerial SensorSerial(D3, D4);  // rx, tx

unsigned long previousMillis = 0;
int distance;
byte state= 3;  // 0-red, 1-yellow, 2-green, 3-blue

void setup() {

  Serial.begin(115200);
  SensorSerial.begin(9600);

  digitalWrite(bluePin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  lcd.begin(16, 2);  // initialize the lcd
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.println("  Water Level");

    WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
}


void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;

  }

  while (SensorSerial.available() > 0) {
    distance =  SensorSerial.parseInt();
    distance = tankLevel - distance;

    if (SensorSerial.read() == '\n') {
      lcd.home();
      lcd.clear();
      lcd.print("  Water Level");
      lcd.setCursor(2, 1);
      lcd.print(float(distance)/100,2);
      lcd.print(" meter");

      ledDisplay();
    }
  }
}

void ledDisplay() {

  digitalWrite(bluePin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);

  if (distance > blueLevel) {
    digitalWrite(bluePin, HIGH);
    if(state!=3){
      state = 3;
      lineNotify();
    }
  } else if (distance > greenLevel) {
    digitalWrite(greenPin, HIGH);
    if(state!=2){
      state = 2;
      lineNotify();
    }
  } else if (distance > yellowLevel) {
    digitalWrite(yellowPin, HIGH);
    if(state!=1){
      state = 1;
      lineNotify();
    }
  } else if (distance > redLevel) {
    digitalWrite(redPin, HIGH);
    if(state!=0){
      state = 0;
      lineNotify();
    }
  }
}

void lineNotify(){
  LINE.notify("ระดับน้ำขณะนี้ " + String(float(distance)/100,2) + " เมตร");
}

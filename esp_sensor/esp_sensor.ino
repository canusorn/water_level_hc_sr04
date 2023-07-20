/*
การต่อสาย

HC-SR04 -> ESP8266
trig  -> D6
echo  -> D5
vcc   -> 5V
GND   -> GND

สายระหว่างบอร์ด -> ESP8266
แดง  ->  V1
ดำ   ->  GND
ฟ้า   ->  D3

*/

// ขาต่อเซนเซอร์ HC-SR04
#define trigPin  D6
#define echoPin  D5

#include <SoftwareSerial.h>

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034

long duration;
int distanceCm;

SoftwareSerial SensorSerial(D4, D3);  // rx, tx

void setup() {
  Serial.begin(115200); // Starts the serial communication
  SensorSerial.begin(9600);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY / 2;


  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  SensorSerial.print(String(distanceCm) + "\n");

  delay(2000);
}

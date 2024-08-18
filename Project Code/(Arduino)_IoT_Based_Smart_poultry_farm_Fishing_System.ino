//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "DHT.h"
#define DHTTYPE DHT11
#define REPORTING_PERIOD_MS     1000
uint32_t tsLastReport = 0;
#define FEEDING_PERIOD_MS     20000
uint32_t tsLastfeed = 0;
Servo myservo1;
Servo myservo2;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int trigPin = 6;
const int echoPin = 5;
const int DHTPIN = 2;
const int waterPin = 7;
const int FanPin = 8;
const int LightPin = 9;
const int PumpPin = 10;
int pos = 0;    // variable to store the servo position
DHT dht(DHTPIN, DHTTYPE);
long duration;
int distance;
void setup()
{
  Serial.begin(9600); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(waterPin, INPUT);
  pinMode(FanPin, OUTPUT);
  pinMode(LightPin, OUTPUT);
  pinMode(PumpPin, OUTPUT);
  digitalWrite(FanPin, HIGH);
  digitalWrite(LightPin, HIGH);
  digitalWrite(PumpPin, HIGH);
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Smart Poultry &");
  lcd.setCursor(0,1);
  lcd.print("Fish Farm System");
   myservo1.attach(4);
   myservo2.attach(3);
   dht.begin();
}


void loop()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  if(digitalRead(waterPin) == HIGH){
    digitalWrite(PumpPin, LOW);
  }
  else{
    digitalWrite(PumpPin, HIGH);
  }
  if(distance == 0){
    distance = 30;
  }
  if (distance < 20){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo2.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo2.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  }
  if (millis() - tsLastfeed > FEEDING_PERIOD_MS){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    tsLastfeed = millis();
  }
  if (millis() - tsLastReport > REPORTING_PERIOD_MS){
    int h = dht.readHumidity();
    int t = dht.readTemperature();
    if(t>33){
      digitalWrite(FanPin, LOW);
      digitalWrite(LightPin, HIGH);
    }
    else if(t<30){
      digitalWrite(FanPin, HIGH);
      digitalWrite(LightPin, LOW);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.print(t);
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print(h);
    String str =String(distance)+String("=")+String(h)+String("=")+String(t);
    Serial.println(str);
    tsLastReport = millis();
  }
}

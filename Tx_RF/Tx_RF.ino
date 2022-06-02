#include <RH_ASK.h> // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h> // Include dependant SPI Library
#include "timer.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;


//distance measurement

#define trigpin 4 // digital pin 4 
#define echopin 3 // digital pin 3


int ena = 5;
int enb = 6;

int in1 = 8;
int in2 = 9;
int in3 = 10;
int in4 = 11;

float distancem;
float Speed;

// Define output strings

String str_dist;
String str_speed;
String str_out;

//speed
Timer timer;

const int LM393 = 2;
int counter = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(ena, 0);
  analogWrite(enb, 0);
  delay(1000);

  // Initialize ASK Object
  rf_driver.init();
  // Setup Serial Monitor
  //Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(LM393), count, RISING);
  //Serial.begin(115200);
  timer.setInterval(3000);
  timer.setCallback(RPM);
  timer.start();

  
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight

}

void count() {
  counter++;
}

float RPM() {
  float rpm = (counter * 60 * 0.1885 * .0025);
  counter = 0;
  return rpm;
}


void loop()
{

  timer.update();
  float Speed = RPM();
  Serial.println(Speed);

  int distance = Usensor();

  if (  (distance < 0)   )
  {
    distance = 0;
  } else if (  (distance >= 0) && (distance <= 40)  )
  {
    analogWrite(ena, 0);
    analogWrite(enb, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  } else

    if (  distance > 40  )
    {
      analogWrite(ena, distance);
      analogWrite(enb, distance);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

  // Convert distance to string
  str_dist = String(distance);

  // Convert Speed to string
  str_speed = String(Speed);

  // Combine Humidity and Temperature
  str_out = str_dist + "," + str_speed;

  // Compose output character
  static char *msg = str_out.c_str();

  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();

  Serial.print("Distance: ");
  Serial.print(str_dist);
  lcd.setCursor(2, 0);
  lcd.print("Distance: ");
  lcd.setCursor(11, 0);
  lcd.print(str_dist);
  lcd.setCursor(15, 0);
  lcd.print("in");
  Serial.print("  - Speed: ");
  Serial.println(str_speed);
  lcd.setCursor(2, 0);
  lcd.print("Speed: ");
  lcd.setCursor(9, 0);
  lcd.print(str_speed);
  lcd.setCursor(14, 0);
  lcd.print("K/h");

  delay(2000);

}



int Usensor() {

  int duration, Distance;
  digitalWrite(trigpin, HIGH);

  delayMicroseconds(1000);
  digitalWrite(trigpin, LOW);


  duration = pulseIn(echopin, HIGH);

  Distance = ( duration / 2) / 29.1;
  Serial.println("inches:");
  Serial.println(Distance);

  Distance = map(Distance, 0 , 197, 0 , 255 );
  return Distance;

}

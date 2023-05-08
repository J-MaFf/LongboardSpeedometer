/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 https://docs.arduino.cc/learn/electronics/lcd-displays

*/

// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, hallSensor = 13;
int currentRpm = 0, count = 0;
double seconds = 0, rpm = 00.00, wheelDiamater = 2.55906, distance = 0, speed = 0;
bool hallSensorValue = 1; // No magnet
bool hallSensorValueOld = 0; // magnet
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/**
 * @brief getRovolutons is a method that Increments count if magnet is detected, representing a full rotation
 * 
 * @param hallSensorValue The current value of the hall sensor
 * @param count The current count of full rotations
 * @return The count of full rotations
*/
int getRevolutions(bool hallSensorValue, int count){
    // Magnet precence outputs 0, no magnet outputs 1
    if (hallSensorValue == 0 && hallSensorValueOld == 1){
        bool tmp = hallSensorValue;
        hallSensorValueOld = hallSensorValue;
        hallSensorValue = tmp;        
        count++;
    }
    if (hallSensorValue == 1 && hallSensorValueOld == 0){
        bool tmp = hallSensorValue;
        hallSensorValueOld = hallSensorValue;
        hallSensorValue = tmp;        
    }
    Serial.print(count);

    return count;
}

/**
 * @brief getRpm is a method that calculates the RPM of the wheel
 * 
 * @param count The current count of full rotations
 * @return The RPM of the wheel
*/

double getRpm(int count, double seconds){
    
  double rpm = (count / seconds) * 60;
  return rpm;
}
/**
 * @brief getSpeed is a method that calculates the speed of the Longboard in MPH based off the rpm and wheel diamater
 * 
 * @param rpm The current RPM of the wheel
 * @param wheelDiamater The diamater of the wheel in mm
 * @return The speed of the Longboard in MPH
 * 
*/
double getSpeed(double rpm, double wheelDiameter) {
  return (rpm * wheelDiameter * PI * 60) / 63360;
}
/**
 * @brief getDistance is a method that calculates the distance traveled based off the speed and time
 * 
 * @param speed The current speed of the Longboard in MPH
 * @param seconds The current time in seconds
 * @return The distance traveled in miles
*/
double getDistance(double speed, double seconds){
  double distance = speed * seconds;
  return distance;
}

void setup() {

  // enable the serial monitor
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Set up hall sensor
  pinMode(hallSensor, INPUT);
}

void loop() {
seconds = millis() / 1000;
//  while untill 5 seconds have passed

bool hallSensorValue = digitalRead(hallSensor);
count = getRevolutions(hallSensorValue, count);
  

if ((int)seconds % 2 == 0)
{
  rpm = getRpm(count, seconds);
  speed = getSpeed(rpm, wheelDiamater);
}

// Printing
lcd.setCursor(0, 0);
lcd.print(speed);
lcd.setCursor(7, 0);
lcd.print("MPH");


lcd.setCursor(13, 0);
lcd.print (millis() / 1000);

lcd.setCursor(0, 1);
lcd.print(rpm);
lcd.setCursor(7, 1);
lcd.print("RPM");
lcd.setCursor(11, 1);
lcd.print("#");
lcd.print(count);
}
/*
Code to control motor using keyboard input via Serial comm.
by Kamalesh Bharadwaj

Ref.: Robojax BTS7960 Motor Control library (https://robojax.com/learn/arduino/?vid=robojax_BTS7960_motor_driver)
Microcontroller: Arduino Mega2560 
*/

#define RPWM 5    // pin for RPWM pin (output)
#define R_EN 7    // pin for R_EN pin (input)
#define R_IS 1    // pin for R_IS pin (output)

#define LPWM 3    // pin for LPWM pin (output)
#define L_EN 6    // pin for L_EN pin (input)
#define L_IS 12   // define pin 8 for L_IS pin (output)

#define CW 1      //do not change
#define CCW 0     //do not change
#define debug 0   //change to 0 to hide serial monitor debugging information or set to 1 to view

#include <RobojaxBTS7960.h>                                     // BTS7960 Motor Control Code by Robojax.com
RobojaxBTS7960 motor(R_EN,RPWM,R_IS, L_EN,LPWM,L_IS,debug); 

const byte ledPin = 13;
int sensorValue = 0; 
int keyp;
int cnt = 0;
int sensorPin = 2;    
      
void setup() {
    motor.begin();
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
   }

void loop() {
   
    if (Serial.available())                     // read keyboard input as serial value 
      {
       keyp = Serial.read();
       if (keyp == 'd')                         //if "d" is pressed on keyboard
          {
           motor.rotate(100,CW);
           delay(25);
           motor.stop();
          }

       if (keyp == 'a')                         //if "a" is pressed on keyboard
          {
           motor.rotate(100,CCW);          
           delay(25);
           motor.stop();
          }  
      }
    sensorValue = digitalRead(sensorPin);       // read the value from the encoder sensor
    if(sensorValue)
      {
       digitalWrite(ledPin, HIGH);
       cnt = cnt + sensorValue;                 // Count sensor triggers
      }
    Serial.println(cnt);                        // Print count value on Serial monitor
  
}

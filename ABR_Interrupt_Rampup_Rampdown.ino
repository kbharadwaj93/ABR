/*
Interrupt based control of Bag Respirator with encoder count, initialization, rampup & rampdown.
AutoBag Respirator Project (2020)
by Suraj Dash & Kamalesh Bharadwaj

Ref. Robojax BTS7960 Motor Control library (https://robojax.com/learn/arduino/?vid=robojax_BTS7960_motor_driver)
Microcontroller: Arduino Mega2560 
*/

#define RPWM 5        // pin for RPWM pin (output)
#define R_EN 7        // pin for R_EN pin (input)
#define R_IS 1        // pin for R_IS pin (output)

#define LPWM 9        // pin for LPWM pin (output)
#define L_EN 6        // pin for L_EN pin (input)
#define L_IS 12       // pin for L_IS pin (output)
#define CW 1          //do not change
#define CCW 0         //do not change
#define debug 0       //change to 0 to hide serial monitor debugging information or set to 1 to view

#include <RobojaxBTS7960.h>                     // BTS7960 Motor Control Code by Robojax.com
RobojaxBTS7960 motor(R_EN,RPWM,0,L_EN,LPWM,0,debug);

const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
volatile int count =0;

int dir = 1;
int endstop=8;
int PWM_B = 40;
int PWM_F = 50;
int PWM = 10;

void setup()
    {
      Serial.begin(9600);
      pinMode(ledPin, OUTPUT);
      pinMode(8, INPUT);
      pinMode(interruptPin, INPUT_PULLUP);
      
      attachInterrupt(digitalPinToInterrupt(interruptPin), encoder, CHANGE);
      motor.begin();       
      Initialize();
      motor.stop();
    }

void loop()
    {
       if (dir==1)                           //Direction- pressing the bag
         {
           rampup(); 
           motor.rotate(PWM,CCW); 
           if (count >= 55)
             {
             motor.stop();
             dir = -1;
             PWM = 10;
             }
           else {}
         }
       else                                  //Direction- releasing the bag
           {
           motor.rotate(PWM,CW);  
           if (count < 5)
             { 
               rampdown();
               if (digitalRead(endstop)== 1)
                  {
                    motor.stop();
                    Serial.println("End Reached! STOPPING");
                    dir = 1;
                    count = 0;
                    PWM = 10;
                  }
               else{}
              }
           else
             {
              rampup();  
             }
         }
    }

void Initialize()                          //bring arm to home position at start
    {
      while(digitalRead(endstop)==1)
              {
                motor.rotate(30,CCW);
                delay(50);
              }
      motor.stop(); 
          
      while(digitalRead(endstop)==0)
              {
                motor.rotate(30,CW);
              }
      Serial.println("End Reached! STOPPING");
      count = 0;
      motor.stop();
    }

void encoder()                            //read encoder interrupt and count 
    {
      state = !state;
      digitalWrite(ledPin, state);
      count = count+dir;
      Serial.println(count);
    }

void rampup()                             //speed up motor gradually to avoid vibrations
    {
      PWM = PWM + 5;
      if (dir == 1)
        {
         if (PWM>=PWM_F)
           {
              PWM = PWM_F;
           }
        }
      if (dir == -1)
        {
         if (PWM>=PWM_B)
           {
              PWM = PWM_B;
           }
        }
    }

void rampdown()                           //slow down motor to avoid vibration also maintain PWM for min torque
    {
      PWM = PWM - 5;
      if (PWM<=15)
         {
            PWM = 15;
         }
    }

/*
Code to check optical encoder performance - slot design vs motor speed - by Kamalesh Bharadwaj
No. of slots/steps counted for different motor speeds and observed on Serial monitor

Ref. Robojax BTS7960 Motor Control library (https://robojax.com/learn/arduino/?vid=robojax_BTS7960_motor_driver)
Microcontroller: Arduino Mega2560 
*/

#define RPWM 6    // pin for RPWM pin (output)
#define R_EN 5    // pin for R_EN pin (input)
#define R_IS 46   // pin for R_IS pin (output)

#define LPWM 7    // pin for LPWM pin (output)
#define L_EN 5    // pin for L_EN pin (input)
#define L_IS 48   // pin for L_IS pin (output)

#define CW 1      //do not change
#define CCW 0     //do not change
#define debug 0   //change to 0 to hide serial monitor debugging infornmation or set to 1 to view

#include <RobojaxBTS7960.h>         // BTS7960 Motor Control Code by Robojax.com
RobojaxBTS7960 motor(R_EN,RPWM,R_IS, L_EN,LPWM,L_IS,debug);


int sensorPin = A0;
int encoder_sensor = 2;
int prev_state = 0; 
int present_state1 = 0;
int pos_count = 0;
int endstop = 4; 

void setup() {
             
          Serial.begin(19200);          
          motor.begin();  
          present_state1 = digitalRead(encoder_sensor);
          while(digitalRead(endstop)==0)            //Motor back to start position(endstop actauted)
             {
              motor.rotate(30,CW);
             }
          motor.stop();

         }


void loop() 
      {
        
       motor.rotate(70,CCW);                         //Motor control - Speed %PWM, direction of roation
       
       prev_state = present_state1;
       present_state1 = digitalRead(encoder_sensor);
       
       if(present_state1!=prev_state)
         {
         pos_count++;                                //Count number of slots detected
         Serial.println(pos_count);                  //To check if number slots crossed is correct
         
         if(pos_count >= 7)   
           {
            motor.stop();
            delay(100);
          
            while(digitalRead(endstop)==0)            //Rotate back until endstop actuated 
                {
                 motor.rotate(30,CW);
                }
            motor.stop();
            delay(100);
           
            pos_count = 0;
            present_state1 = digitalRead(encoder_sensor);
            
           }    
         }         
      }

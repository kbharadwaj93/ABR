/* 
	Debounce.h for managing mechanical switch oscillations and find true HIGH/LOW
	Part of Arduino Library for Automated Bag Respirator(ABR) Project (2020).
	
*/

#ifndef debounce_h
#define debounce_h

#include "Arduino.h"

class DebouncedButton 
    {
      const unsigned long DebounceDelay = 100;

      public:
      DebouncedButton(const int& pin);
      bool is_HIGH();

      private:
      int pin1;
      unsigned long last_high_time = 0;

    };


bool DebouncedButton::is_HIGH() 
    {
      int reading = digitalRead(pin1);
      bool high_value = false;
      const unsigned long time_now = millis();
      if (reading == HIGH) 
        {
         if ((time_now - last_high_time) > DebounceDelay) 
            {
              high_value = true;
            }
         last_high_time = time_now;
        }
      return high_value;
    }

#endif


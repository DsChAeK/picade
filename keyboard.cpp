#include "keyboard.h"
#include "config.h"

#ifndef ENABLE_JOYSTICK

void InitKeyboard()
{
  for(int i = 0; i < sizeof(inputs_key) / sizeof(input_key); i++)
  {
    pinMode(inputs_key[i].pin, INPUT_PULLUP);
  }
}

void CheckKeyboard()
{
  boolean changed = false; // has any input changed?
    
  // loop through each input
  for(int i = 0; i < sizeof(inputs_key) / sizeof(input_key); i++)
  {
    // test for current state of this input
    boolean state = !digitalRead(inputs_key[i].pin);

    if(state != inputs_key[i].state && (millis() - inputs_key[i].last_change) > DELAY_BUTTON_DEBOUNCE) // has this input changed state since the last time we checked?
    {
      inputs_key[i].state = state; // update our state map so we know what's happening with this key in future
      inputs_key[i].last_change = millis();
      
      // send the key press or release event
      if(state)
      { 
        Keyboard.press(inputs_key[i].key);
      }
      else
      {
        Keyboard.release(inputs_key[i].key);
      }
    }
  }
}
#endif

// Configuration
#include "config.h"

// Modules
#include "volume.h"   
#include "keyboard.h" 
#include "joystick.h"

// Libraries
#include "TimerOne.h"
#include <EEPROM.h>

void setup() 
{ 
  // Init LED
  pinMode(BLINK_LED, OUTPUT);
  
  // Init volume
  InitVolume();

  // Init controls
  #ifdef ENABLE_JOYSTICK
    InitJoystick();
  #else
    InitKeyboard();
  #endif

  // Start timer for checking controls
  InitTimer(timer, 5000);
}

void loop() 
{
  // Check volume
  CheckVolume();
}

void timer(void)
{
  // Check controls
  #ifdef ENABLE_JOYSTICK
    CheckJoystick();
  #else
    CheckKeyboard();
  #endif
}


#ifndef keyboard_h_
#define keyboard_h_

#include <arduino.h>


// Input data
typedef struct
{
  char    key;      // keycode to emit when input is pressed
  char    pin;      // Arduino pin number for this input
  boolean state;    // what state was the input last in ( HIGH/LOW )
  unsigned long last_change;
  
} input_key;

// Public functions
void InitKeyboard(void);
void CheckKeyboard(void);

#endif


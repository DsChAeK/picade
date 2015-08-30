#include "joystick.h"
#include "config.h"

#ifdef ENABLE_JOYSTICK

/*
 * SNES pinout and wiring
 * 
 * ref: http://www.repairfaq.org/REPAIR/F_SNES.html
 * 
    1 VCC   -> 5V
    2 Clock -> Discrete 0
    3 Latch -> Discrete 1
    4 Data  -> Discrete 2
    7 GND   -> GND
 * */
#define SNES_DATA 2
#define SNES_LATCH 1 // Also ISR 3
#define SNES_CLOCK 0 // Also ISR 2

// Joystick USB data structure, which stores the status of each button, and the X/Y axis values.
JoyState_t joySt = { .data = {0, 0, 0, 0} };

// Initial states for all buttons. 'High' means button is NOT pressed.
volatile SnesPad_t snesdata = {.data = 0xFFFF };
volatile int shiftreg = 0;

/** ISR when the SNES LATCH line is set to HIGH. Stores the buttons into
 * the shift register and prepares the first value on the data line.*/
void latchvalues()
{
    shiftreg = snesdata.data;
    digitalWrite(SNES_DATA, shiftreg & 1);
    shiftreg = shiftreg >> 1;
}

/** ISR for the SNES CLOCK pulse. Shifts the next value onto the data line.
 * Note this is technically the same clock pulse as the SNES samples at,
 * but with ISR overhead, we will always be after the sample and thus preparing
 * the NEXT value.*/
void shiftvalues()
{
    digitalWrite(SNES_DATA, shiftreg & 1);
    shiftreg = shiftreg >> 1;
}

void InitJoystick()
{  
  // Configure all button and joystick discrete input pins
  pinMode(DPAD_LEFT, INPUT_PULLUP);
  pinMode(DPAD_UP, INPUT_PULLUP);
  pinMode(DPAD_RIGHT, INPUT_PULLUP);
  pinMode(DPAD_DOWN, INPUT_PULLUP);

  pinMode(BUTTON_Y, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_X, INPUT_PULLUP);
  pinMode(BUTTON_L, INPUT_PULLUP);
  pinMode(BUTTON_R, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_START, INPUT_PULLUP);
  
  // Configure the SNES input/output pins and ISR settings
  pinMode(SNES_LATCH, INPUT_PULLUP);
  pinMode(SNES_CLOCK, INPUT_PULLUP);
  pinMode(SNES_DATA, OUTPUT);

  // Setup interrupts
  attachInterrupt(2, shiftvalues, FALLING); // Clock ISR
  attachInterrupt(3, latchvalues, RISING); // Latch ISR
    
  // Init joystick, send first (neutral) data values for the joystick
  joySt.xAxis = 127;
  joySt.yAxis = 127;
  
  Joystick.setState(&joySt);
}


/** Continually acquires the button/joystick states into the USB and SNES structs. 
 *  The USB data report is sent every frame. The SNES data is latched and clocked
 *  out by the appropriate ISRs.
 * */
void CheckJoystick()
{
  // Aquire and store the SNES button states into the SNES data struct
  snesdata.button_X = digitalRead(BUTTON_X);
  snesdata.button_Y = digitalRead(BUTTON_Y);
  snesdata.button_A = digitalRead(BUTTON_A);
  snesdata.button_B = digitalRead(BUTTON_B);
  snesdata.button_L = digitalRead(BUTTON_L);
  snesdata.button_R = digitalRead(BUTTON_R);
  snesdata.button_Select = digitalRead(BUTTON_SELECT);
  snesdata.button_Start = digitalRead(BUTTON_START);
  snesdata.dpad_up = digitalRead(DPAD_UP);
  snesdata.dpad_down = digitalRead(DPAD_DOWN);
  snesdata.dpad_left = digitalRead(DPAD_LEFT);
  snesdata.dpad_right = digitalRead(DPAD_RIGHT);
  
  // Acquire and store the button states into the USB data struct
  joySt.button_Y = !snesdata.button_Y;
  joySt.button_B = !snesdata.button_B;
  joySt.button_A = !snesdata.button_A;
  joySt.button_X = !snesdata.button_X;
  joySt.button_L = !snesdata.button_L;
  joySt.button_R = !snesdata.button_R;
  joySt.button_Select = !snesdata.button_Select;
  joySt.button_Start = !snesdata.button_Start;
 
  // Convert the digital joystick position into analog X/Y axis
  if (!snesdata.dpad_left)
      joySt.xAxis = 0;
  else if (!snesdata.dpad_right)
      joySt.xAxis = 255;
  else
      joySt.xAxis = 127;

  if (!snesdata.dpad_up)
      joySt.yAxis = 0;
  else if (!snesdata.dpad_down)
      joySt.yAxis = 255;
  else
      joySt.yAxis = 127;
 
  /* Timing statistics for the USB call:
   * - If USB Connected and utilized: ~1 ms (total frame time)
   * - If USB Connected and idle: ~500 ms (total frame time)
   * - If USB Disconnected entirely: ~25 us function call; ~100 us total frame time
   * 
   * This is also why there is no debouncing; worst case bounce is measured at ~ 10 us
   * (See SDS00009), which is less than one frame. Even with the fastest SNES frame time,
   * it will not be possible to detect both the high and low parts of a bounce.
   * That's not even taking into account the fact that the SNES only samples every
   * 16.67 ms (60 Hz).
   * Extensive testing with R-Type 3 also did not register any nuisance shots when releasing
   * a charged shot.
   */
  Joystick.setState(&joySt);
       
  delay(DELAY_BUTTON_DEBOUNCE);
}

#endif

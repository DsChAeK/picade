#ifndef joystick_h_
#define joystick_h_

#include <arduino.h>

// Input data
typedef union SnesPad
{
    struct
    {
        // Clock Data to the right/LSB, but board is Little Endian. 
        // Top entries are therefore clocked first
        uint16_t   button_B: 1;
        uint16_t   button_Y: 1;
        uint16_t   button_Select: 1;
        uint16_t   button_Start: 1;
        uint16_t   dpad_up: 1;
        uint16_t   dpad_down: 1;
        uint16_t   dpad_left: 1;
        uint16_t   dpad_right: 1;
        uint16_t   button_A: 1;
        uint16_t   button_X: 1;
        uint16_t   button_L: 1;
        uint16_t   button_R: 1;
        uint16_t   button_Coin: 1;
        uint16_t   button_1Up: 1;

        #ifndef ENABLE_VOLUME_BUTTONS
        uint16_t   button_L1: 1;
        uint16_t   button_R1: 1;
        #else
        uint16_t   padding: 2;
        #endif
    };
    
    uint16_t data;

} SnesPad_t;

// Public functions
void InitJoystick(void);
void CheckJoystick(void);

#endif

#ifndef config_h_
#define config_h_

#include "keyboard.h"
#include "joystick.h"


// ########################
// ### General settings ###
// ########################

#define ENABLE_VOLUME_BUTTONS    // Uncomment to bind the volume buttons to keys
#define ENABLE_JOYSTICK          // Uncomment to use joystick instead of keyboard control

#define DELAY_VOLUME_AMP      40 // Delay(ms) for set volume amplifier
#define DELAY_BUTTON_DEBOUNCE 50 // Delay(ms) for debouncing buttons

#define VOLUME_MAX_GAIN       20 // Set maximum volume level (0-VOLUME_MAX_GAIN_STEPS)
#define VOLUME_STARTUP_GAIN    5 // Set startup volume (0-VOLUME_MAX_GAIN). Set -1 to save and load the last volume.


// ####################
// ### Pin settings ###
// ####################

// Pin LED
#define BLINK_LED   13 // PC7

// Pins buttons & joystick
#define BTN_1      A5  // PF0
#define BTN_2      A4  // PF1
#define BTN_3      A3  // PF4
#define BTN_4      A2  // PF5
#define BTN_5      A1  // PF6
#define BTN_6      A0  // PF7
#define LEFT       10  // PB6
#define RIGHT       9  // PB5
#define UP          6  // PD7
#define DOWN        8  // PB4

// Pins system controls
#define START      12  // PD6
#define COIN        4  // PD4      (1/4) on silk
#define ESCAPE      1  // PD3/TX
#define ENTER       0  // PD2/RX    1UP on silk

// Pins audio & amplifier
#define VOL_UP      3  // PD0
#define VOL_DN      2  // PD1
#define HEADPHONE_DETECT 5  // PC6
#define AMP_UP      11 // PB7
#define AMP_DN       7 // PB6


// #########################
// ### Keyboard settings ###
// #########################

static input_key inputs_key[] =         
{
  { KEY_UP_ARROW,    UP     },
  { KEY_DOWN_ARROW,  DOWN   },
  { KEY_LEFT_ARROW,  LEFT   },
  { KEY_RIGHT_ARROW, RIGHT  },

  { 'a',             BTN_1  }, // Control panel
  { 's',             BTN_2  },
  { 'd',             BTN_3  },
  { 'z',             BTN_4  },
  { 'x',             BTN_5  },
  { 'c',             BTN_6  },

  { 'q',             START  }, // Front side
  { 'w',             COIN   },
  
  { KEY_LEFT_CTRL,   ENTER  }, // Left side
  { KEY_ESC,         ESCAPE },
  
#ifndef ENABLE_VOLUME_BUTTONS
  /* Change these lines to set key bindings for VOL_UP and VOL_DN */
  { 'u',             VOL_UP }, // Right side     
  { 'd',             VOL_DN },
#endif
};


// #########################
// ### Joystick settings ###
// #########################

#define DPAD_UP        UP
#define DPAD_DOWN      DOWN
#define DPAD_LEFT      LEFT
#define DPAD_RIGHT     RIGHT

#define BUTTON_Y       BTN_1
#define BUTTON_B       BTN_2
#define BUTTON_A       BTN_3
#define BUTTON_X       BTN_4
#define BUTTON_L       BTN_5
#define BUTTON_R       BTN_6

#define BUTTON_START   ENTER
#define BUTTON_SELECT  ESCAPE

#endif

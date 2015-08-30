#ifndef volume_h_
#define volume_h_

#define VOLUME_MAX_GAIN_STEPS 32 // There are 32 discrete gain settings from +24dB maximum to -80dB minimum, used for volume_current_reset()
 
// Public functions
void InitVolume(void);
void CheckVolume(void);

#endif

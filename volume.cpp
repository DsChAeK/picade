#include "config.h"
#include "volume.h"
#include <EEPROM.h>

volatile int volume_current = 0; // volume currently set to amp
int volume_target           = 0; // volume we want to reach
int volume_saved            = 0; // volume saved in eeprom
int volume_eeprom           = 0; // volume loaded from eeprom

// Internal functions
void volume_target_save();
void volume_target_load();

void volume_current_up();
void volume_current_down();
void volume_current_fade(int level);
void volume_current_reset();

void volume_eeprom_load();
void volume_eeprom_save(int target);

void volume_target_save()
{
  volume_saved = volume_target;
}

void volume_target_load()
{
  volume_target = volume_saved;
  
  if( volume_target > VOLUME_MAX_GAIN ) 
    volume_target = VOLUME_MAX_GAIN;
}

void volume_current_up()
{
  if(volume_current > VOLUME_MAX_GAIN) 
    return;
    
  volume_current++;
  digitalWrite(AMP_UP, LOW);
  delay(DELAY_VOLUME_AMP);
  digitalWrite(AMP_UP, HIGH);
  delay(5);
};

void volume_current_down()
{ 
  if(volume_current == 0) 
    return;
    
  volume_current--;
  digitalWrite(AMP_DN, LOW);
  delay(DELAY_VOLUME_AMP);
  digitalWrite(AMP_DN, HIGH);
  delay(5);
};

void volume_current_fade(int level)
{
  if(level > volume_current)
  {
    while(level > volume_current)
    {
      volume_current_up();
    }
  }
  else
  {
    while(level < volume_current)
    {
      volume_current_down();
    }
  }
}

void volume_current_reset()
{
  volume_current=VOLUME_MAX_GAIN_STEPS+1; // we go all steps down!
  
  while(volume_current > 0)
  {
    volume_current_down();
  }
}

void volume_eeprom_load()
{  
  volume_target = EEPROM.read(0) & 0b00011111;
  volume_eeprom = volume_target;  
}

void volume_eeprom_save(int target)
{
  EEPROM.write(0, target);
 
  volume_eeprom = target;
}

// Init volume on startup
void InitVolume(void)
{  
  pinMode(HEADPHONE_DETECT, INPUT);

#ifdef ENABLE_VOLUME_BUTTONS
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DN, INPUT_PULLUP);
#endif
  
  pinMode(AMP_UP, OUTPUT);
  digitalWrite(AMP_UP, HIGH);
  pinMode(AMP_DN, OUTPUT);
  digitalWrite(AMP_DN, HIGH);

  // set default volume for startup
  if (VOLUME_STARTUP_GAIN > 0)
  {
    if (VOLUME_STARTUP_GAIN > VOLUME_MAX_GAIN)
    {
      volume_eeprom_save(VOLUME_MAX_GAIN);
    }
    else
    {
      volume_eeprom_save(VOLUME_STARTUP_GAIN);
    }
  }
   
  // reset volume
  volume_current_reset();
     
  // load volume from eeprom
  volume_eeprom_load();

  // set current volume    
  volume_current_fade(volume_eeprom);
}

void CheckVolume(void)
{
  #ifdef ENABLE_VOLUME_BUTTONS
  static unsigned long last_vol_up_time = 0;
  static unsigned long last_vol_dn_time = 0;
  
  static boolean last_vol_up    = 0;
  static boolean last_vol_dn    = 0;
  static boolean last_headphone = 0;

 /*
  * Read the current states of the Volume Up, Volume Down and Headphone Detect inputs
  */
  boolean vol_up    = !digitalRead(VOL_UP);
  boolean vol_dn    = !digitalRead(VOL_DN);
  boolean headphone = !digitalRead(HEADPHONE_DETECT);
  
  /*
   * If headphones are unplugged/plugged in then adjust the volume accordingly.
   * Save the target volume and set it to 0 to fade out.
   * Load the saved value into the target volume to fade back in.
   */
  if(headphone != last_headphone)
  {
    last_headphone = headphone;
    
    if(headphone)
    {
      volume_target_save();
      volume_target = 0;
    }
    else
    {    
      volume_target_load();
    }
  }
  
  /* 
     Inc/Dec volume if direction has changed or at least 100ms
     have passed and volume_target is not reached yet.
  */
  
  if( vol_up != last_vol_up || (millis() - last_vol_up_time) > 100 )
  {
    if( vol_up && (volume_target < VOLUME_MAX_GAIN) )
    {
      volume_target++;
      last_vol_up_time = millis();
    }
    
    last_vol_up = vol_up;
  }

  if( vol_dn != last_vol_dn || (millis() - last_vol_dn_time) > 100 )
  {
    if( vol_dn && volume_target > 0 )
    {
      volume_target--;
      last_vol_dn_time = millis();
    }
    
    last_vol_dn = vol_dn;
  }

  /* Inc/Dec volume one step to target */
  if(volume_target > volume_current)
  {
    volume_current_up();
  }
  
  if(volume_target < volume_current)
  {
    volume_current_down();
  }

  /* If we've reached the target volume, save it to EEPROM and blink the notification LED */
  if(!headphone && (volume_current == volume_target) && 
                   (volume_eeprom != volume_current) && 
                   ((millis() - last_vol_up_time) > 1000) &&
                   ((millis() - last_vol_dn_time) > 1000))
  {
    // set current volume
    volume_eeprom_save(volume_current);      

    int x = 2;
    
    while(x--)
    {
      digitalWrite(BLINK_LED, HIGH);
      delay(50);
      digitalWrite(BLINK_LED, LOW);
      delay(50);
    }
  }
#endif

}

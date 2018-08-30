#define IN_DANGER_NUM 10
#define ACHE_BEAT 100
#define HEARTACHE 3
#define IN_DANGER 4

#include <Arduino.h>

#include "main.h"
#include "SENSOR.h"
#include "HELP.h"

int danger = 0;

extern all_data data;

int what_help(void)
{
  if(data.heart.beat < ACHE_BEAT)
    return HEARTACHE;

  if(data.gyro.roll > (int8_t)40 || data.gyro.roll < (int8_t)-40 || data.gyro.pitch > (int8_t)40 || data.gyro.pitch < (int8_t)-40)
    danger++;

  if(danger == IN_DANGER_NUM)
  {
    danger = 0;
    
    return IN_DANGER;
  }
  return 0;
}

void in_danger(int pin)
{   
  Serial2.flush();
  
  digitalWrite(pin, HIGH);
  
  while(Serial2.available() <= 0);
    
  Serial2.read();
    
  while(1)
  {
    get_sensor_data();
    
    Serial2.write(data.heart.beat);
    Serial2.write(data.gyro.roll);
    Serial2.write(data.gyro.pitch);
    
    while(Serial2.available() <= 0);
    
    if(Serial.read() == 'X')
      break;
  }

  digitalWrite(pin, LOW);
}

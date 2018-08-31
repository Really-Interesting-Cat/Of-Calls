#define IN_DANGER_NUM 10
#define ACHE_BEAT 100
#define HEARTACHE 2
#define IN_DANGER 3

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "main.h"
#include "SENSOR.h"
#include "HELP.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, 6, NEO_GRB + NEO_KHZ800);

int danger = 0;

extern all_data data;

int safe_led_on = 0;
int ache_led_on = 0;
int danger_led_on = 0;

static int i = 0;

void led_strip_init(void)
{
  strip.begin(); 
  strip.show();
}

void in_danger_led(int pin)
{
  if(ache_led_on == 0 && pin == HEARTACHE)
  {
      for(i = 0; i < 6; i++)
      {
        strip.setPixelColor(i,strip.Color(50,  0,  0));  
        strip.show();
      }
      
      safe_led_on = 0;
      danger_led_on = 0;
      ache_led_on = 1;  
  }

  else if(danger_led_on == 0 && pin == IN_DANGER)
  {
      for(i = 0; i < 6; i++)
      {
        strip.setPixelColor(i,strip.Color(50,  35,  0));  
        strip.show();

      } 
      
      safe_led_on = 0;
      ache_led_on = 0;
      danger_led_on = 1;   
  }
}

void safe_led(void)
{
    for(i = 0; i < 6; i++)
    {
      strip.setPixelColor(i,strip.Color(0,  50,  0));  
      strip.show();
    }

    ache_led_on = 0;
    danger_led_on = 0;
    safe_led_on = 1;
}

int what_help(void)
{
  if(data.heart.user == 0)
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
  digitalWrite(pin + 6, HIGH);
  
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
  digitalWrite(pin + 6, LOW);
}

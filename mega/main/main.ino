#define HEARTACHE 2
#define IN_DANGER 3

#include <Wire.h>
#include <SPI.h>

#include "main.h"
#include "HELP.h"
#include "SENSOR.h"

struct all_data data;
int help = 0;
int counter = 0;

extern int MPU;

extern int delay_time;

extern int safe_led_on;

ISR(SPI_STC_vect)
{
  switch(counter)
  {
    case 0:
      SPDR = data.heart.beat;
      counter = 1;
      break;
    case 1:
      SPDR = data.gyro.roll;
      counter = 2;
      break;
    case 2:
      SPDR = data.gyro.pitch;
      counter = 0;
      break;
  }
}

void setup() 
{
  pinMode(HEARTACHE, OUTPUT);
  pinMode(IN_DANGER, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(HEARTACHE + 6, OUTPUT);
  pinMode(IN_DANGER + 6, OUTPUT);

  digitalWrite(HEARTACHE, LOW);
  digitalWrite(IN_DANGER, LOW);
  digitalWrite(HEARTACHE + 6, LOW);
  digitalWrite(IN_DANGER + 6, LOW);
  
  Wire.begin();      //Wire 라이브러리 초기화
  Wire.beginTransmission(MPU); //MPU로 데이터 전송 시작
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     //MPU-6050 시작 모드로 만들기
  Wire.endTransmission(true);

  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);
  
  SPI.setClockDivider(SPI_CLOCK_DIV64); // 250khz

  SPCR |= _BV(SPE); 
  SPCR &= ~_BV(MSTR); 
  SPCR |= _BV(SPIE); 

  Serial1.begin(9600);

  led_strip_init();

  safe_led();
}

void loop() 
{
  get_sensor_data();

  if(data.heart.user == (uint8_t)0x01)
    help = what_help();
 
  if(safe_led_on == 0 && help == 0) 
      safe_led();
  
  else if(help == HEARTACHE || help == IN_DANGER)
  {
    delay_time = 0;
    in_danger(help);
    delay_time = 1;
  
    help = 0;
  }
}

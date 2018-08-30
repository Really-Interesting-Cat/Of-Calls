#define HEARTACHE 3
#define IN_DANGER 4

#include <Wire.h>

#include "main.h"
#include "HELP.h"
#include "SENSOR.h"

struct all_data data;
int help = 0;

extern int MPU;

extern int delay_time;

void setup() 
{
  pinMode(HEARTACHE, OUTPUT);
  pinMode(IN_DANGER, OUTPUT);

  digitalWrite(HEARTACHE, LOW);
  digitalWrite(IN_DANGER, LOW);
  
  Wire.begin();      //Wire 라이브러리 초기화
  Wire.beginTransmission(MPU); //MPU로 데이터 전송 시작
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     //MPU-6050 시작 모드로 만들기
  Wire.endTransmission(true);
  
  Serial1.begin(9600);
  Serial2.begin(9600);
}

void loop() 
{ 
  get_sensor_data();

  if(data.heart.user == (uint8_t)0x01)
    help = what_help();
  
  if(help == HEARTACHE || help == IN_DANGER)
  {
    delay_time = 0;
    in_danger(help);
    delay_time = 1;

    help = 0;
  }
}

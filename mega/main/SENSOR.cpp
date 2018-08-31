#include <Arduino.h>
#include <Wire.h>

#include "main.h"
#include "SENSOR.h"

int delay_time = 1;

int MPU = 0x68;  //MPU 6050 의 I2C 기본 주소 설정
int16_t AcX,AcY,AcZ_X=12700, AcZ_Y=12800;

extern all_data data;

void get_bt_data(void)
{
  uint8_t bt_data;
  
  Serial1.flush();
     
  while(Serial1.available() <= 0)
    Serial1.write('A');
 
  bt_data = (uint8_t)Serial1.read();

  data.heart.user = (uint8_t)(bt_data & (uint8_t)0x01);
  data.heart.beat = (uint8_t)((bt_data & (uint8_t)0xFE) >> 1);
}

void get_gyro_data(void)
{
  Wire.beginTransmission(MPU);    //데이터 전송시작
  Wire.write(0x3B);               // register 0x3B (ACCEL_XOUT_H), 큐에 데이터 기록
  Wire.endTransmission(false);    //연결유지
  Wire.requestFrom(MPU,6,true);   //MPU에 데이터 요청
//------------------------------------------------------------------------------------------------------
  //데이터 한 바이트 씩 읽어서 반환
  AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  
  double RADIAN_TO_DEGREES = 180/3.14;
  double val_x = atan(AcY/sqrt(pow(AcX,2) + pow(AcZ_X,2))) * RADIAN_TO_DEGREES; //보정전 Roll값 구하기
  double val_y = atan(AcX/sqrt(pow(AcY,2) + pow(AcZ_Y,2))) * RADIAN_TO_DEGREES; //보정전 Pitch값 구하기
//------------------------------------------------------------------------------------------------------
  val_x = floor(val_x+0.5);  //val_x값 보정
  val_x += 52;             //val_x값의 음수값 삭제  

  double roll = 90.0/104.0*val_x;

  if(roll == 45)
  {
    roll = 0;
  }
  
  else if(roll > 45)
  {
    if(roll > 90)
    {
      roll = -90;
    }
    
    else
    {
      roll -= 46; 
      roll = 90.0/44.0*roll;
      roll *= -1;
    }
  }
  
  else if(roll < 45)
  {
    if(roll < 0)
    {
      roll = 90;
    }

    else
    {
      roll = 90.0/44.0*roll-90;
      roll *= -1;
    }
  }
   roll = floor(roll);
//------------------------------------------------------------------------------------------------------  
  val_y = floor(val_y);  //val_y값 보정
  val_y += 52;         //val_y값의 음수값 삭제

  double pitch = 90.0/104.0*val_y;
  
  if(pitch == 45)
  {
    pitch = 0;
  }
  
  else if(pitch > 45)
  {
    if(pitch > 90)
    {
      pitch = 90;
    }

    else
    {
      pitch -= 46; 
      pitch = 90.0/44.0*pitch;
    }
  }
  
  else if(pitch < 45)
  {
    if(pitch < 0)
    {
      pitch = -90;
    }

    else
    {
      pitch = 90.0/44.0*pitch-90;
    }
  }
   pitch = floor(pitch);

   data.gyro.roll = (int8_t)roll;
   data.gyro.pitch = (int8_t)pitch;
}

void get_sensor_data(void)
{
  get_bt_data();
  get_gyro_data();
  
  if(delay_time == 1)
      delay(500);
  
}

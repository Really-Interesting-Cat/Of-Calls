#define HEARTACHE 3
#define IN_DANGER 4

#define INT_PIN 2

#include <Wire.h>

#include "main.h"
#include "HELP.h"
#include "SENSOR.h"
#include "gps.h"

struct all_data data;
int help = 0;
int counter = 0;

extern int MPU;

extern int delay_time;

extern int safe_led_on;

extern volatile int exception;

void setup() 
{
  pinMode(HEARTACHE, OUTPUT);
  pinMode(IN_DANGER, OUTPUT);
  pinMode(HEARTACHE + 6, OUTPUT);
  pinMode(IN_DANGER + 6, OUTPUT);
  pinMode(INT_PIN, INPUT);

  digitalWrite(HEARTACHE, LOW);
  digitalWrite(IN_DANGER, LOW);
  digitalWrite(HEARTACHE + 6, LOW);
  digitalWrite(IN_DANGER + 6, LOW);
  
  Wire.begin();      //Wire 라이브러리 초기화
  Wire.beginTransmission(MPU); //MPU로 데이터 전송 시작
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     //MPU-6050 시작 모드로 만들기
  Wire.endTransmission(true);

  init_gps();

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(115200);
  Serial3.begin(9600);

  led_strip_init();

  safe_led();

  Serial.println("Hello World!");
 
  get_sensor_data(); // 안정화

  Serial.println("Moudle get session ended");

  attachInterrupt(digitalPinToInterrupt(INT_PIN), IRQ_DANGER, FALLING);

  Serial.println("Inuterrupt Initialized");
}

void loop() 
{
  get_sensor_data();
  
  help = what_help();

  if(exception == 1)
  {
    exception = 0;
    help = IN_DANGER;
  }

  switch(help)
  {
    default:
      Serial.println("Don't need help");
      break;
    case HEARTACHE:
      Serial.println("HEARTACHE");
      break;
    case IN_DANGER:
      Serial.println("IN_DANGER");
      break;
  }
 
  if(safe_led_on == 0 && help == 0) 
      safe_led();
  
  else if(help == HEARTACHE || help == IN_DANGER)
  {
    delay_time = 0;
    in_danger(help);
    delay_time = 1;
  }
}

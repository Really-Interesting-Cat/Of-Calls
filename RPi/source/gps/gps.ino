#include ".\gps.h"

unsigned short i, addr = 0;

void setup() {
  // put your setup code here, to run once:
  init_gps();
  Serial2.begin(115200);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial2.available()){
    addr += (i = Serial2.readBytes(gps_packet_buff + addr, 1));
    if(addr == 36){
        if(gps_packet_buff[0] == 0xB5 && gps_packet_buff[1] == 0x62){
          for(int j = 0; j < addr; j++){
            Serial.print(gps_packet_buff[j]);
            Serial.print(" ");
           }
          Serial.println(' ');
          check_gps_packet();
    
          if(checksum_flag){
            decode_gps_posllh_packet();
            Serial.print("iTow: ");Serial.println(data.iTow);
            Serial.print("hAcc: ");Serial.println(data.hAcc);
            Serial.print("vAcc: ");Serial.println(data.vAcc);
            Serial.print("longitude: ");Serial.println(data.longitude);
            Serial.print("latitude: ");Serial.println(data.latitude);
            Serial.print("height: ");Serial.println(data.height);
            Serial.print("hMSL: ");Serial.println(data.hMSL);
          }
        }
      addr = 0;
    }
  }
}

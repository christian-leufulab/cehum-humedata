/*
   TITLE: CEHUM - HUMEDAT@ XI'AN
   AUTHOR: CHRISTIAN SANTIBÁÑEZ SOTO
   COMPANY: LEUFÜLAB
   DATE: 11/07/2022
   VERSION: 1
*/
 #include "libs.h"

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  delay(1000);
  pinMode(xian_switch, OUTPUT);
  pinMode(gps_switch, OUTPUT);
  digitalWrite(xian_switch, LOW);
  digitalWrite(gps_switch, LOW);
  delay(500);
  lorawan_begin();
  sd_begin();
  Wire.begin();
}

void loop() {
  get_gps_data();
  env_pressure();
  get_atm_values();
  read_battery_level();
  read_xian_sensors();
  
  store_sd_data();
  send_lorawan_data();

  //delay((sleep_time - gps_fix_time)*60*1000);
  LowPower.sleep((sleep_time - gps_fix_time)*60*1000);
  digitalWrite(gps_switch, HIGH);
  delay(1000);
  //delay(gps_fix_time*60*1000);
  LowPower.sleep(gps_fix_time*60*1000);
}

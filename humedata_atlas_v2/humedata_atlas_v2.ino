/* TITLE: CEHUM - HUMEDAT@ ATLAS
   AUTHOR: CHRISTIAN SANTIBÁÑEZ SOTO
   COMPANY: LEUFÜLAB
   DATE: 18/10/2022
   VERSION: 2.0 */
   
const int SLEEP_TIME = 2;
#include "libs.h"

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  Serial1.begin(9600);
  while(!Serial1);
  pinMode(ATLAS_SWITCH, OUTPUT);
  pinMode(GPS_SWITCH, OUTPUT);
  pinMode(ATLAS_SWITCH, HIGH);
  Wire.begin();
  sd_init();
}

void loop() 
{
  read_atlas_sensors();
  env_pressure();
  get_atm_values();
  digitalWrite(ATLAS_SWITCH, LOW);
  get_gps_data();
  read_battery_level();
  print_data();
  write_to_sd(_data[0],_data[1],_data[2],_data[3],_data[4],_data[5],_data[6],_data[7],
              _data[8],_data[9],_data[10],_data[11], _data[12], _data[13], _data[14], 
              _data[15], _data[16], _data[17], _data[18], _data[19], _data[20], _data[21]);
  generate_lorawan_array();
  send_lorawan_data();
  LowPower.sleep(SLEEP_TIME*60*1000);
}

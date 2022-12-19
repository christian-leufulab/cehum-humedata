/*  TITLE: CEHUM - HUMEDAT@ XI'AN
    AUTHOR: CHRISTIAN SANTIBÁÑEZ SOTO
    COMPANY: LEUFÜLAB
    DATE: 11/07/2022
    VERSION: 1 */
    
 #include "libs.h"

// Sleep time in minutes
const int sleep_time = 10; 
const int gps_fix_time = 3;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  declare_pins();
  lorawan_begin();
  sd_begin();
  Wire.begin();
}

void loop() {
  env_pressure();
  get_atm_values();
  read_battery_level();
  read_xian_sensors();
  store_sd_data();
  send_lorawan_data();

  //delay((sleep_time - gps_fix_time)*60*1000);
  LowPower.sleep((sleep_time - gps_fix_time)*60*1000);
  digitalWrite(GPS_SWITCH, HIGH);
  //delay(gps_fix_time*60*1000);
  LowPower.sleep(gps_fix_time*60*1000);
  get_gps_data();
}

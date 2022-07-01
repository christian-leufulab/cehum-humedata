#include <Wire.h>
#include <Arduino_MKRENV.h>
#include <MKRWAN.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_BMP280.h>
#include <RTClib.h>
#include "ArduinoLowPower.h"
#include "TinyGPS++.h"

#include "io_definitions.h"
#include "wtr_functions.h"
#include "int_functions.h"
#include "atm_functions.h"


/* --------------------------- Variables --------------------------- */

float measures_raw[7] = {};
int switch_pin = 1;
int sensor_transmission_time = 2000;
int sensor_warmup_time = 10000;
int update_measure_time = 10000;
/* --------------------------- Implicit Functions --------------------------- */

void init_machine();

void sensors_state(bool state);

void update_values();
void print_values();

void update_values();


void do_wire_transmission(){
  Wire.beginTransmission(do_address);
  Wire.write("r");
  Wire.endTransmission();
  delay(do_time);
  Wire.requestFrom(do_address, 20, 1);
  do_code = Wire.read();
  while(Wire.available()){
    do_in_char = Wire.read();
    do_data[do_i] = do_in_char;
    do_i++;
    if(do_in_char == 0){
      do_i = 0;
      break;
    }
  }

  DO = strtok(do_data, ",");
  sat = strtok(NULL, ",");  

  /*
   * DEBUGGING
   */
//  Serial.print("-- DISSOLVED OXYGEN: ");
  measures_raw[0] = (float)atof(DO);
  measures_raw[1] = (float)atof(sat);
//  Serial.print(_data[0]);
//  Serial.println(" --");

}

void ph_wire_transmission(){
  Wire.beginTransmission(ph_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(ph_time);
  Wire.requestFrom(ph_address, 20, 1);
  ph_code = Wire.read();

  while(Wire.available()){
    ph_in_char = Wire.read();
    ph_data[ph_i] = ph_in_char;
    ph_i++;
    if(ph_in_char == 0){
      ph_i = 0;
      break;
    }
  }
//  Serial.print("-- PH: ");
  measures_raw[2] = (float)atof(ph_data);
//  Serial.print(_data[1]);
//  Serial.println(" --");
}


void ec_wire_transmission(){
  Wire.beginTransmission(ec_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(ec_time);
  Wire.requestFrom(ec_address, 32, 1);
  ec_code = Wire.read();
  while(Wire.available()){
    ec_in_char = Wire.read();
    ec_data[ec_i] = ec_in_char;
    ec_i++;
    if(ec_in_char == 0){
      ec_i = 0;
      break;
    }
  }
  
  ec = strtok(ec_data, ",");
  tds = strtok(NULL, ",");
  sal = strtok(NULL, ",");
  sg = strtok(NULL, ",");

//  Serial.print("-- ELECTRICAL CONDUCTIVITY ");
//  Serial.print("EC: ");
  measures_raw[3] = (float)atof(ec);
//  Serial.print(_data[2]);
//  Serial.print(" || TDS: ");
  measures_raw[4] = (float)atof(tds);
//  Serial.print(_data[3]);
//  Serial.print(" || SAL: ");
  measures_raw[5] = (float)atof(sal);
//  Serial.print(_data[4]);
//  Serial.print(" || SG: ");
  measures_raw[6] = (float)atof(sg);
//  Serial.print(_data[5]);
//  Serial.println(" --");
}


void setup() {
  init_machine();
  
  

}

void loop() {
  update_values();
  print_values();

  delay(update_measure_time);
}

/* --------------------------- Explicit Functions --------------------------- */

void init_machine(){
  Serial.begin(115200);
  Serial1.begin(9600);
  Wire.begin();

  pinMode(switch_pin, OUTPUT);

  
  digitalWrite(switch_pin, HIGH);


  delay(1000);
  
  if (!modem.begin(AU915)) {
    Serial.println("-- NO SE HA PODIDO INICIAR EL MÓDULO LORAWAN --");
    while (1) {}
  };
  
  Serial.print("-- LA VERSIÓN DE TU MÓDULO ES: ");
  Serial.print(modem.version());
  Serial.println(" --");
  
  Serial.print("-- EL EUI DE TU DISPOSITIVO ES: ");
  Serial.print(modem.deviceEUI());
  Serial.println(" --");

  Serial.println(modem.sendMask("ff000001f000ffff00020000"));
  int connected = modem.joinOTAA(appEui, appKey);

  Serial.print("-- CONNECTED STATUS: ");
  Serial.print(connected);
  Serial.println(" --");

  modem.minPollInterval(300);

}

void sensors_state(bool state){
  digitalWrite(switch_pin, state);
}

void update_values(){   //get values and save in the measures_raw
  sensors_state(LOW);
  Serial.print("\r\nUpdate values ... \r\n");
  
  delay(sensor_warmup_time);
  
  do_wire_transmission();
  delay(sensor_transmission_time);
  ph_wire_transmission();
  delay(sensor_transmission_time);
  ec_wire_transmission();
  delay(sensor_transmission_time);
  
  for(uint8_t i=0; i<sizeof(measures_raw)/sizeof(measures_raw[0]); i++){
    
    Serial.print(i);
  }

  sensors_state(HIGH);
}


void print_values(){
  

  Serial.print("\r\nMeasures: ");
  for(uint8_t i=0; i<sizeof(measures_raw)/sizeof(measures_raw[0]); i++){
    Serial.print("[");
    Serial.print(measures_raw[i]);
    Serial.print("]");
    if(i<(sizeof(measures_raw)/sizeof(measures_raw[0])) - 1){
      Serial.print(", ");
    }
  }
  Serial.print("\r\n");
  
  
}

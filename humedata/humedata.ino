/*
   TITLE: CEHUM - HUMEDAT@ ATLAS
   AUTHOR: CHRISTIAN SANTIBÁÑEZ SOTO
   COMPANY: LEUFÜLAB
   DATE: 12/01/2022
   VERSION: 1
*/

// Librerías
#include <Wire.h>
#include <Arduino_MKRENV.h>
#include <SPI.h>
#include <SD.h>
#include <MKRWAN.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_BMP280.h>
#include <RTClib.h>
#include "ArduinoLowPower.h"
#include "TinyGPS++.h"
#include "io_definitions.h"

// Unidades de medida
/*
  [0]  --> DISSOLVED OXYGEN          [mg/L]
  [1]  --> PH                        [-]
  [2]  --> ELECTRICAL CONDUCTIVITY   [uS/cm]
  [3]  --> TOTAL DISSOLVED SOLIDS    [ppm]
  [4]  --> SALINITY                  [ppt]
  [5]  --> RELATIVE DENSITY          [-]
  [6]  --> WATER TEMPERATURE         [°C]
  [7]  --> INTERNAL PRESSURE         [KPa]
  [8]  --> ATMOSPHERIC PRESSURE      [KPa]
  [9]  --> ATMOSPHERIC TEMPERATURE   [°C]
  [10] --> GPS LATITUDE              [°]
  [11] --> GPS LONGITUDE             [°]
  [12] --> INTERNAL TEMPERATURE      [°C]
  [13] --> INTERNAL HUMIDITY         [%]
  [14] --> BATTERY LEVEL             [V]
  [15] --> ORP                       [mV]
*/

// TIEMPOS DE SLEEP Y DE CALENTAMIENTO (MINUTOS)
const int sleep_time = 10; 

void setup() {
  Serial.begin(115200);
  pinMode(off_pin, OUTPUT);
  digitalWrite(off_pin, LOW);
  
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

//   modem.minPollInterval(300);

//  Serial1.begin(9600);

//  delay(10000);
//  turn_on_gps();
  
  SPI.begin();
  delay(100);
  SD.begin(sd_cs_pin);
  dataFile = SD.open("log-0000.csv", FILE_WRITE);
  delay(1000);
  dataFile.println("DissolvedOxygen,pH,ElectricalConductivity,TotalDissolvedSolids,Salinity,RelativeDensity,WaterTemperature,InternalPressure,AtmosphericPressure,AtmosphericTemperature,Latitude,Longitude,InternalTemperature,InternalHumidity,BatteryLevel,ORP");
  dataFile.close();
  delay(100);
  SPI.end();
}

void loop() {
  digitalWrite(off_pin, HIGH);
  
  do_wire_transmission();

  ph_wire_transmission();

  ec_wire_transmission();

  orp_wire_transmission();

  //get_water_temp();

  rtd_wire_transmission();

  env_pressure();

  get_atm_values();

  //get_gps_data();

  read_battery_level();

  Serial.print("DATOS: [");
  for (int i = 0; i < _data_size ; i++) {
    Serial.print(_data[i]);
    if (i < _data_size -1) {
      Serial.print(", ");
    }
  }
  Serial.println("]");

  write_to_sd(_data[0],_data[1],_data[2],_data[3],_data[4],_data[5],_data[6],_data[7],_data[8],_data[9],_data[10],_data[11], _data[12], _data[13], _data[14], _data[15]);
  

  _data_lorawan[0]  = uint8_t  (_data[0]  * 1);              // Dissolved Oxygen           
  _data_lorawan[1]  = uint8_t  (_data[1]  * 255/14);         // pH                         
  _data_lorawan[2]  = uint8_t  (_data[2]  * 1);              // Electrical Conductivity    
  _data_lorawan[3]  = uint8_t  (_data[3]  * 1);              // Total Dissolved Solids    
  _data_lorawan[4]  = uint8_t  (_data[4]  * 1);              // Salinity                  
  _data_lorawan[5]  = uint8_t  (_data[5]  * 1);              // Relative Density         
  _data_lorawan[6]  = uint8_t  (_data[6]  * 1);              // Water Temperature          
  _data_lorawan[7]  = uint8_t  (_data[7]  * 1);              // Internal Pressure         
  _data_lorawan[8]  = uint8_t  (_data[8]  * 1);              // Atmospheric Pressure      
  _data_lorawan[9]  = uint8_t  (_data[9]  * 1);              // Atmospheric Temperature   
  _data_lorawan[10] = uint16_t (_data[10] * 1);              // GPS Latitude               
  _data_lorawan[11] = uint16_t (_data[11] * 1);              // GPS Longitude              
  _data_lorawan[12] = uint8_t  (_data[12] * 1);              // Internal Temperature      
  _data_lorawan[13] = uint8_t  (_data[13] * 1);              // Internal Humidity         
  _data_lorawan[14] = uint8_t  (_data[14] * 1);              // Battery Level              
  _data_lorawan[15] = uint16_t (_data[15] * 1);              // ORP

  int err;
  modem.beginPacket();
  modem.write(_data_lorawan[0]);
  modem.write(_data_lorawan[1]);
  modem.write(_data_lorawan[2]);
  modem.write(_data_lorawan[3]);
  modem.write(_data_lorawan[4]);
  modem.write(_data_lorawan[5]);
  modem.write(_data_lorawan[6]);
  modem.write(_data_lorawan[7]);
  modem.write(_data_lorawan[8]);
  modem.write(_data_lorawan[9]);
  modem.write(_data_lorawan[10]);
  modem.write(_data_lorawan[11]);
  modem.write(_data_lorawan[12]);
  modem.write(_data_lorawan[13]);
  modem.write(_data_lorawan[14]);
  modem.write(_data_lorawan[15]);
  
  err = modem.endPacket(true);

  if (err > 0) {
    Serial.println("-- MENSAJE ENVIADO CORRECTAMENTE A TRAVÉS DE LORAWAN --");
  } else {
    Serial.println("-- ERROR ENVIANDO EL MENSAJE A TRAVÉS DE LORAWAN --");
  }

  sleep_sensors();
  digitalWrite(off_pin, LOW);
  LowPower.sleep(sleep_time*60*1000); // 10 minutos * 60*segundos * 1000 milisegundos
}

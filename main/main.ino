/*
   TITLE: CEHUM - HUMEDATA
   AUTHOR: CHRISTIAN SANTIBÁÑEZ SOTO
   COMPANY: LEUFÜLAB
   DATE: 23/04/2021
   VERSION: 0.5
*/

/*
   LIBRARIES
*/
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
#include "IO_Definitions.h"

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

void setup() {
  Serial.begin(115200);
  
  if (!modem.begin(AU915)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  Serial.println(modem.sendMask("ff000001f000ffff00020000"));
  int connected = modem.joinOTAA(appEui, appKey);

  Serial.print("-- CONNECTED STATUS: ");
  Serial.print(connected);
  Serial.println(" --");

  modem.minPollInterval(300);

  Serial1.begin(9600);

  delay(10000);
  turn_on_gps();
  
  SPI.begin();
  delay(100);
  SD.begin(sd_cs_pin);
  dataFile = SD.open("log-0000.csv", FILE_WRITE);
  delay(1000);
  dataFile.println("DissolvedOxygen,pH,ElectricalConductivity,TotalDissolvedSolids,Salinity,RelativeDensity,WaterTemperature,InternalPressure,AtmosphericPressure,AtmosphericTemperature,Latitude,Longitude,InternalTemperature,InternalHumidity,BatteryLevel,ORP");
  dataFile.close();
  delay(100);
}

void loop() {
  do_wire_transmission();

  ph_wire_transmission();

  ec_wire_transmission();

  orp_wire_transmission();

  //get_water_temp();

  rtd_wire_transmission();

  env_pressure();

  //get_atm_values();

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
  

  _data_lorawan[0]  = uint8_t(_data[0]  * 1);              // Dissolved Oxygen           K: 
  _data_lorawan[1]  = uint8_t(_data[1]  * 255/14);         // pH                         K: 0.055
  _data_lorawan[2]  = uint8_t(_data[2]  * 1);              // Electrical Conductivity    K: ((70/255)-20)
  _data_lorawan[3]  = uint8_t(_data[3]  * 1);              // Total Dissolved Solids     K:
  _data_lorawan[4]  = uint8_t(_data[4]  * 1);              // Salinity                   K:
  _data_lorawan[5]  = uint8_t(_data[5]  * 1);              // Relative Density           K:
  _data_lorawan[6]  = uint8_t(_data[6]  * 1);              // Water Temperature          K:
  _data_lorawan[7]  = uint8_t(_data[7]  * 1);              // Internal Pressure          K:
  _data_lorawan[8]  = uint8_t(_data[8]  * 1);              // Atmospheric Pressure       K:
  _data_lorawan[9]  = uint8_t(_data[9]  * 1);              // Atmospheric Temperature    K:
  _data_lorawan[10] = uint8_t(_data[10] * 1);              // GPS Latitude               K:
  _data_lorawan[11] = uint8_t(_data[11] * 1);              // GPS Longitude              K:
  _data_lorawan[12] = uint8_t(_data[12] * 1);              // Internal Temperature       K:
  _data_lorawan[13] = uint8_t(_data[13] * 1);              // Internal Humidity          K:
  _data_lorawan[14] = uint8_t(_data[14] * 1);              // Battery Level              K:
  _data_lorawan[15] = uint8_t(_data[15] * 1);              // ORP

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
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }

  sleep_sensors();
  
  LowPower.sleep(60*5*1000);
}

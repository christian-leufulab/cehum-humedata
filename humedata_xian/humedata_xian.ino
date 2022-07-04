/*
   TITLE: CEHUM - HUMEDAT@ XI'AN
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
#include <Adafruit_BMP280.h>
#include <RTClib.h>
#include "ArduinoLowPower.h"
#include "TinyGPS++.h"
#include "io.h"
#include "vars.h"
#include "wtr.h"
#include "atm.h"
#include "int.h"

/*
 *           UNIDADES DE MEDIDA
 *           
 *[0]  --> DISSOLVED OXYGEN          [mg/L]
 *[1]  --> PH                        [-]
 *[2]  --> ELECTRICAL CONDUCTIVITY   [uS/cm]
 *[3]  --> TOTAL DISSOLVED SOLIDS    [ppm]
 *[4]  --> SALINITY                  [ppt]
 *[5]  --> RELATIVE DENSITY          [-]
 *[6]  --> WATER TEMPERATURE         [°C]
 *[7]  --> INTERNAL PRESSURE         [hPa]
 *[8]  --> ATMOSPHERIC PRESSURE      [hPa]
 *[9]  --> ATMOSPHERIC TEMPERATURE   [°C]
 *[10] --> GPS LATITUDE              [°]
 *[11] --> GPS LONGITUDE             [°]
 *[12] --> INTERNAL TEMPERATURE      [°C]
 *[13] --> INTERNAL HUMIDITY         [%]
 *[14] --> BATTERY LEVEL             [V]
 *[15] --> ORP                       [mV]
 */

/*        RANGO SENSORES
 *         
 * DISSOLVED OXYGEN        --> 0 - 100 [mg/L]     [1 byte]  [O]
 * pH                      --> 0 - 14             [1 byte]  [O]
 * ELECTRICAL CONDUCTIVITY --> 0,07 - 500.000     [4 bytes] [O]
 * TOTAL DISSOLVED SOLIDS  --> 5 - 500.000        [4 bytes] [O]
 * SALINITY                --> 0,00 - 42,00       [1 byte]  [O]
 * RELATIVE DENSITY        --> 1,00 - 1,300       [1 byte]  [O]
 * WATER TEMPERATURE       --> 0 - 60             [1 byte]  [O]
 * INTERNAL PRESSURE       --> 80 - 200           [1 byte]  [O]
 * ATMOSPHERIC PRESSURE    --> 80 - 120           [1 byte]  [O]
 * ATMOSPHERIC TEMPERATURE --> -20 - 60           [1 byte]  [O]     
 * GPS LATITUDE            -->                    [4 bytes] [O]
 * GPS LONGITUDE           -->                    [4 bytes] [O]
 * INTERNAL TEMPERATURE    --> -20 - 60           [1 byte]  [O]
 * INTERNAL HUMIDITY       --> 0 - 120            [1 byte]  [O]
 * BATTERY LEVEL           --> 8 - 13             [1 byte]  [O]
 * ORP                     --> -2000 - 2000       [4 bytes] [O]
 * 
 * TOTAL:                                         [31 bytes]
 */

void float2Bytes(float val,byte* bytes_array){
  // Create union of shared memory space
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

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

  Serial1.begin(9600);

  Wire.begin();
  
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
  env_pressure();

  get_atm_values();

  get_gps_data();

  read_battery_level();

  Serial.println("FLOAT DATA:");
  Serial.print(" [");
  for (int i = 0; i < _data_size ; i++) {
    Serial.print(_data[i]);
    if (i < _data_size -1) {
      Serial.print(", ");
    }
  }
  Serial.println("]");

  write_to_sd(_data[0],_data[1],_data[2],_data[3],_data[4],_data[5],_data[6],_data[7],_data[8],_data[9],_data[10],_data[11], _data[12], _data[13], _data[14], _data[15], _data[16]);

  float2Bytes(gps_latitude,&gps_latitude_float_bytes[0]);
  float2Bytes(gps_longitude,&gps_longitude_float_bytes[0]);
  

  Serial.println("LORAWAN HEX DATA: ");
  
  for(int a = 0; a < sizeof(_data_lorawan); a++)
  {
    Serial.print("0x");
    Serial.print(_data_lorawan[a], HEX);
    Serial.print(" ");
  }

  Serial.println();

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
  modem.write(_data_lorawan[16]);
  modem.write(_data_lorawan[17]);
  modem.write(_data_lorawan[18]);
  modem.write(_data_lorawan[19]);
  modem.write(_data_lorawan[20]);
  modem.write(_data_lorawan[21]);
  modem.write(_data_lorawan[22]);
  modem.write(_data_lorawan[23]);
  modem.write(_data_lorawan[24]);
  modem.write(_data_lorawan[25]);
  modem.write(_data_lorawan[26]);
  modem.write(_data_lorawan[27]);
  modem.write(_data_lorawan[28]);
  modem.write(_data_lorawan[29]);
  modem.write(_data_lorawan[30]);
  modem.write(_data_lorawan[31]);
  modem.write(_data_lorawan[32]);
  modem.write(_data_lorawan[33]);
  
  err = modem.endPacket(true);

  if (err > 0) {
    Serial.println("-- MENSAJE ENVIADO CORRECTAMENTE A TRAVÉS DE LORAWAN --");
  } else {
    Serial.println("-- ERROR ENVIANDO EL MENSAJE A TRAVÉS DE LORAWAN --");
  }

  delay(3*60*1000);
  //LowPower.sleep(sleep_time*60*1000); // 10 minutos * 60*segundos * 1000 milisegundos
}

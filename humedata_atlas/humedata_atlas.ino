/*
   TITLE: CEHUM - HUMEDAT@ ATLAS
   AUTHOR: CHRISTIAN SANTIBÁÑEZ SOTO
   COMPANY: LEUFÜLAB
   DATE: 12/01/2022
   VERSION: 1.0
*/

// Librerías
#include <Wire.h>
#include <Arduino_MKRENV.h>
#include <SPI.h>
#include <SD.h>
#include <MKRWAN.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <Adafruit_BMP280.h>
//#include <RTClib.h>
#include "ArduinoLowPower.h"
#include "TinyGPS++.h"
#include "io_definitions.h"


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
 *[7]  --> INTERNAL PRESSURE         [KPa]
 *[8]  --> ATMOSPHERIC PRESSURE      [KPa]
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




// TIEMPOS DE SLEEP (MINUTOS)
const int sleep_time = 15; 

// GPS Object
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(rtd_off_pin, OUTPUT);
  pinMode(ph_off_pin, OUTPUT);
  pinMode(orp_off_pin, OUTPUT);
  pinMode(ec_off_pin, OUTPUT);
  pinMode(do_off_pin, OUTPUT);
  
  digitalWrite(rtd_off_pin, LOW);
  digitalWrite(ph_off_pin, LOW);
  digitalWrite(orp_off_pin, LOW);
  digitalWrite(ec_off_pin, LOW);
  digitalWrite(do_off_pin, LOW);

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

//  delay(10000);
//  turn_on_gps();

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
  digitalWrite(rtd_off_pin, HIGH);
  digitalWrite(ph_off_pin, HIGH);
  digitalWrite(orp_off_pin, HIGH);
  digitalWrite(ec_off_pin, HIGH);
  digitalWrite(do_off_pin, HIGH);

  delay(1500);
  
  do_wire_transmission();

  delay(1000);

  ph_wire_transmission();

  delay(1000);

  ec_wire_transmission();

  delay(1000);

  orp_wire_transmission();

  delay(1000);

  rtd_wire_transmission();

  delay(1000);

  env_pressure();

  get_atm_values();

  get_gps_data();

  read_battery_level();

  sleep_sensors();
  digitalWrite(rtd_off_pin, LOW);
  digitalWrite(ph_off_pin, LOW);
  digitalWrite(orp_off_pin, LOW);
  digitalWrite(ec_off_pin, LOW);
  digitalWrite(do_off_pin, LOW);

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
  float2Bytes(_data[2] /*EC*/,&ec_float_bytes[0]);
  float2Bytes(_data[3] /*TDS*/,&tds_float_bytes[0]);
  float2Bytes(_data[15] /*ORP*/,&orp_float_bytes[0]);
  float2Bytes(_data[0] /*DO*/,&do_float_bytes[0]);
  //float2Bytes(_data[16] /*SAT*/,&sat_float_bytes[0]);
  

  _data_lorawan[0]  = do_float_bytes[0];                            // DO 
  _data_lorawan[1]  = do_float_bytes[1];                            // DO 
  _data_lorawan[2]  = do_float_bytes[2];                            // DO 
  _data_lorawan[3]  = do_float_bytes[3];                            // DO 
    
  _data_lorawan[4]  = uint8_t  (_data[1]  * 255/14.0);              // pH   
                       
  _data_lorawan[5]  =   ec_float_bytes[0];                          // Electrical Conductivity
  _data_lorawan[6]  =   ec_float_bytes[1];                          // Electrical Conductivity
  _data_lorawan[7]  =   ec_float_bytes[2];                          // Electrical Conductivity
  _data_lorawan[8]  =   ec_float_bytes[3];                          // Electrical Conductivity
  
  _data_lorawan[9]   =   tds_float_bytes[0];                         // Total Dissolved Solids    
  _data_lorawan[10]  =   tds_float_bytes[1];                        // Total Dissolved Solids    
  _data_lorawan[11]  =   tds_float_bytes[2];                        // Total Dissolved Solids    
  _data_lorawan[12]  =   tds_float_bytes[3];                        // Total Dissolved Solids   
   
  _data_lorawan[13]  = uint8_t  (_data[4]  * 255/42.0);             // Salinity  
     
  _data_lorawan[14]  = uint8_t  ((_data[5] - 1) * 255/0.3);         // Relative Density   
   
  _data_lorawan[15]  = uint8_t  (_data[6]  * 255/60.0);             // Water Temperature 
  
  _data_lorawan[16]  = uint8_t  ((_data[7] - 80) * 255/120.0);      // Internal Pressure
    
  _data_lorawan[17]  = uint8_t  ((_data[8]  - 80) * 255/40.0);      // Atmospheric Pressure 
   
  _data_lorawan[18]  = uint8_t  ((_data[9] + 20) * 255/80.0);       // Atmospheric Temperature   
  
  _data_lorawan[19] =   gps_longitude_float_bytes[0];               // GPS Longitude      
  _data_lorawan[20] =   gps_longitude_float_bytes[1];               // GPS Longitude  
  _data_lorawan[21] =   gps_longitude_float_bytes[2];               // GPS Longitude   
  _data_lorawan[22] =   gps_longitude_float_bytes[3];               // GPS Longitude   
   
  _data_lorawan[23] =   gps_latitude_float_bytes[0];                // GPS Latitude 
  _data_lorawan[24] =   gps_latitude_float_bytes[1];                // GPS Latitude 
  _data_lorawan[25] =   gps_latitude_float_bytes[2];                // GPS Latitude 
  _data_lorawan[26] =   gps_latitude_float_bytes[3];                // GPS Latitude 
  
  _data_lorawan[27] = uint8_t  ((_data[12] + 20) * 255/80.0);       // Internal Temperature
  
  _data_lorawan[28] = uint8_t  (_data[13] * 255/120.0);             // Internal Humidity 
  
  _data_lorawan[29] = uint8_t  ((_data[14] - 536) * 255/168);       // Battery Level 
  
  _data_lorawan[30] =   orp_float_bytes[0];                         // ORP
  _data_lorawan[31] =   orp_float_bytes[1];                         // ORP
  _data_lorawan[32] =   orp_float_bytes[2];                         // ORP
  _data_lorawan[33] =   orp_float_bytes[3];                         // ORP
  
//  _data_lorawan[34] =   sat_float_bytes[0];                         // SAT
//  _data_lorawan[35] =   sat_float_bytes[1];                         // SAT
//  _data_lorawan[36] =   sat_float_bytes[2];                         // SAT
//  _data_lorawan[37] =   sat_float_bytes[3];                         // SAT

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

  
//  modem.write(_data_lorawan[34]);
//  modem.write(_data_lorawan[35]);
//  modem.write(_data_lorawan[36]);
//  modem.write(_data_lorawan[37]);
  
  
  
  
  err = modem.endPacket(true);

  if (err > 0) {
    Serial.println("-- MENSAJE ENVIADO CORRECTAMENTE A TRAVÉS DE LORAWAN --");
  } else {
    Serial.println("-- ERROR ENVIANDO EL MENSAJE A TRAVÉS DE LORAWAN --");
  }

  
  
  //delay(360*1000);
  LowPower.sleep(sleep_time*60*1000); // 10 minutos * 60*segundos * 1000 milisegundos
}

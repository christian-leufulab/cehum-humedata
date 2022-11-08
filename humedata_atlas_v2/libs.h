#include <Wire.h>
#include <Arduino_MKRENV.h>
#include <SPI.h>
#include <SD.h>
#include <MKRWAN.h>
#include <Adafruit_BMP280.h>
#include "ArduinoLowPower.h"
#include "TinyGPS++.h"
#include "io.h"
#include "vars.h"
#include "int.h"
#include "atm.h"
#include "wtr.h"

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

/*
   LORAWAN DATA VARIABLES
*/
LoRaModem modem;
// String devEUI = "A8610A3233298409"; // --> ALPHA 
// String devEUI = "A8610A3237277009"; // --> ATLAS 
String appEui = "70B3D57ED0042E7E";
String appKey = "5ED7E15D4D31804F8E2A8C447CAE76CC";
uint8_t _data_lorawan[16];

/*
   RTC DS3231
*/
RTC_DS3231 rtc;

/*
   EXTERNAL PRESSURE SENSOR BMP280
*/
Adafruit_BMP280 bmp;
float atm_pressure;
float atm_temperature;

/*
   WATER TEMPERATURE SENSOR DS18B20
*/
const int water_temp_pin = 3;
OneWire oneWire(water_temp_pin);
DallasTemperature sensors(&oneWire);
float water_temperature;

/*
    RTD WATER TEMPERATURE SENSOR
 */
const int rtd_address = 102;
byte rtd_code = 0;                  
char rtd_data[20];          
byte rtd_in_char = 0;            
byte rtd_i = 0;                    
int rtd_time_ = 600;              
float rtd_tmp_float;              

/*
   PH SENSOR VARIABLES
*/
const int ph_address = 99;
byte ph_code = 0;
char ph_data[20];
byte ph_in_char = 0;
byte ph_i = 0;
int ph_time = 815;

/*
   ORP SENSOR VARIABLES
 */
const int orp_address = 98;
byte orp_code = 0;
char orp_data[20];
byte orp_in_char = 0;
byte orp_i = 0;
int orp_time = 815; 

/*
   ELECTRIC CONDUCTIVITY SENSOR VARIABLES
*/
const int ec_address = 100;
byte ec_code = 0;
char ec_data[32];
byte ec_in_char = 0;
byte ec_i = 0;
int ec_time = 570;
char *ec;
char *tds;
char *sal;
char *sg;

/*
   DISSOLVED OXYGEN SENSOR VARIABLES
*/
const int do_address = 97;
char *DO;
char *sat;
byte do_code = 0;
char do_data[20];
byte do_in_char = 0;
byte do_i = 0;
int do_time = 575;

/*
   MKR ENV SENSOR VARIABLES
*/
float internal_pressure = 0;
float internal_temperature = 0;
float internal_humidity = 0;

const int sd_cs_pin = 4;
File dataFile;
float sd_data[14];

/*
   SIM808 VARIABLES
*/
char gps_data[128];
byte gps_code = 0;
byte gps_in_char = 0; 
byte gps_i = 0;

char *gps_left_waste;
char *gps_lat;
char *gps_lon;

float gps_latitude = 0;
float gps_longitude = 0;

/*
   BATTERY LEVEL
*/
int batt_level = 0;
float batt_level_float = 0.0;

/*
   GENERAL VARIABLES
*/
const int _data_size = 16;
float _data[_data_size];

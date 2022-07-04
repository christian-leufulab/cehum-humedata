// LoRaWAN
LoRaModem modem;
// String devEUI = "A8610A3233298409"; // --> ALPHA 
// String devEUI = "A8610A3237277009"; // --> ATLAS 
String devEUI = "A8610A3237267209"; // --> XI'AN
String appEui = "70B3D57ED0042E7E";
String appKey = "5ED7E15D4D31804F8E2A8C447CAE76CC";
uint8_t _data_lorawan[34];

// Sleep time in minutes
const int sleep_time = 15; 

// BMP280
Adafruit_BMP280 bmp;
float atm_pressure;
float atm_temperature;

// MKR ENV Shield
float internal_pressure = 0;
float internal_temperature = 0;
float internal_humidity = 0;
File dataFile;
float sd_data[16];
const int _data_size = 17;
float _data[_data_size];

// Battery Level Reading
int batt_level = 0;
float batt_level_float = 0.0;

// GPS 
TinyGPSPlus gps;
float gps_latitude = 0.0;
float gps_longitude = 0.0;
byte gps_latitude_float_bytes[4];
byte gps_longitude_float_bytes[4];

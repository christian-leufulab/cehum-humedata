// LoRaWAN
LoRaModem modem;
// String devEUI = "A8610A3233298409"; // --> ALPHA 
// String devEUI = "A8610A3237277009"; // --> ATLAS 
String devEUI = "A8610A3237267209"; // --> XI'AN
String appEui = "70B3D57ED0042E7E";
String appKey = "1195AF29AFFE1665C5AF311B4DE62D5A";
uint8_t _data_lorawan[49];

// BMP280
Adafruit_BMP280 bmp;
float atm_pressure;
float atm_temperature;

// MKR ENV Shield
float internal_pressure = 0;
float internal_temperature = 0;
float internal_humidity = 0;
File dataFile;
float sd_data[17];
const int _data_size = 17;
float _data[_data_size];

// Battery Level Reading
int batt_reading = 0;
  float batt_voltage = 0.0;

// GPS 
TinyGPSPlus gps;
float gps_latitude = 0.0;
float gps_longitude = 0.0;
byte gps_latitude_float_bytes[4];
byte gps_longitude_float_bytes[4];

// UART commands to request sensors values
byte request_ec[]   = {0x13, 0x03, 0x26, 0x00, 0x00, 0x05, 0x8d, 0xf3}; 
byte request_ph[]   = {0x01, 0x03, 0x28, 0x00, 0x00, 0x02, 0xcd, 0xab};
byte request_do[]   = {0x04, 0x03, 0x20, 0x00, 0x00, 0x06, 0xce, 0x5d};
byte request_orp[]  = {0x07, 0x03, 0x12, 0x00, 0x00, 0x02, 0xc1, 0x15};

// Buffer to store readings from sensors
byte ec_readings[50];
byte ph_readings[50];
byte do_readings[50];
byte orp_readings[50];

// Variables to store water readings
float ec_f   = 0.0;
float ph_f   = 0.0;
float do_f   = 0.0;
float orp_f  = 0.0;
float temp_f = 0.0;
float sat_f   = 0.0;
byte tds_float_bytes[4];
float tds = 0;

int do_received = 0;
int ph_received = 0;
int ec_received = 0;
int orp_received = 0;

int reading_tries = 0;

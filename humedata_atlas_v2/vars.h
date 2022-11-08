TinyGPSPlus gps;
LoRaModem modem;
//String appEui = "70B3D57ED0042E7E";                     // --> ATLAS
//String appKey = "5ED7E15D4D31804F8E2A8C447CAE76CC";     // --> ATLAS
String appEui = "A8610A3237267209";                     // --> BP
String appKey = "EF09882529BA1054A2062ACDFBDB30EC";     // --> BP
//String appEui = "70B3D57ED0042E7E";                     // --> ALPHA
//String appKey = "5ED7E15D4D31804F8E2A8C447CAE76CC";     // --> ALPHA

uint8_t _data_lorawan[49];

Adafruit_BMP280 bmp;
float atm_pressure;
float atm_temperature;

const int rtd_address = 102;
byte rtd_code = 0;                  
char rtd_data[20];          
byte rtd_in_char = 0;            
byte rtd_i = 0;                    
int rtd_time_ = 600;              
float rtd_tmp_float;       
String inst_temp = "25.0";       

const int ph_address = 99;
byte ph_code = 0;
char ph_data[20];
char ph_data_temp[20];
byte ph_in_char = 0;
byte ph_i = 0;
int ph_time = 815;

const int orp_address = 98;
byte orp_code = 0;
char orp_data[20];
byte orp_in_char = 0;
byte orp_i = 0;
int orp_time = 815; 
byte orp_float_bytes[4];

const int ec_address = 100;
byte ec_code = 0;
char ec_data[32];
byte ec_in_char = 0;
byte ec_i = 0;
int ec_time = 570;
char *ec;
char *ec_temp;
char *tds;
char *sal;
char *sg;
byte tds_float_bytes[4];
byte ec_float_bytes[4];
byte ec_temp_float_bytes[4];

const int do_address = 97;
char *DO;
char *DO_temp;
char *DO_15;
char *sat;
char *sat_temp;
char *sat_15;
byte do_code = 0;
char do_data[20];
byte do_in_char = 0;
byte do_i = 0;
int do_time = 575;
byte do_float_bytes[4];
byte sat_float_bytes[4];
byte do_temp_float_bytes[4];
byte sat_temp_float_bytes[4];
byte do_15_float_bytes[4];

float internal_pressure = 0.0;
float internal_temperature = 0.0;
float internal_humidity = 0.0;

File dataFile;
float sd_data[22];

float gps_latitude = 0;
float gps_longitude = 0;
byte gps_latitude_float_bytes[4];
byte gps_longitude_float_bytes[4];

int batt_level = 0;
float batt_level_float = 0.0;

const int _data_size = 22;
float _data[_data_size];

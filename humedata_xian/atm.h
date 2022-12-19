void get_atm_values(){
  // Se cambia la dirección por defecto del BMP280 (0x77) por (0x76)
  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   
  Adafruit_BMP280::SAMPLING_X2,                    
  Adafruit_BMP280::SAMPLING_X16,                    
  Adafruit_BMP280::FILTER_X16,                    
  Adafruit_BMP280::STANDBY_MS_500);               
  atm_pressure = bmp.readPressure()/1000;
  atm_temperature = bmp.readTemperature();
  
  _data[8] = atm_pressure;  
  _data[9] = atm_temperature;
}

void get_gps_data(){
  // Se apagan los sensores Xi'An y se enciende el GPS
  digitalWrite(XIAN_SWITCH, LOW);
  digitalWrite(RS485_SWITCH, LOW);
  digitalWrite(GPS_SWITCH, HIGH);
  while(Serial1.available() > 0){
    if(gps.encode(Serial1.read())){
      gps_latitude = gps.location.lat();
      gps_longitude = gps.location.lng();
    }
  }
  _data[11] = gps_longitude;
  _data[10] = gps_latitude;
  // Se apaga el GPS
  digitalWrite(GPS_SWITCH, LOW);
}

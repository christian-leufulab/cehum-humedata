void get_atm_values(){
  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  atm_pressure = bmp.readPressure()/1000;
  atm_temperature = bmp.readTemperature();
  
//  Serial.print("-- EXTERNAL PRESSURE: ");
  _data[8] = atm_pressure;
//  Serial.print(atm_pressure);
//  Serial.println(" KPA --");
  
  
//  Serial.print("-- EXTERNAL TEMPERATURE: ");
  _data[9] = atm_temperature;
//  Serial.print(atm_temperature);
//  Serial.println(" °C --");

}

void turn_on_gps(){
  Serial1.println("AT+CGPSPWR=1");
}

void get_gps_data(){
  Serial1.println("AT+CGPSINF=0");

  if(Serial1.available() > 0){
    byte num_chars = Serial1.readBytes(gps_data, 128);
//    Serial.println(num_chars);
    gps_data[num_chars] = '\0';
//    Serial.println(gps_data);

    gps_left_waste = strtok(gps_data, ",");
    gps_lon = strtok(NULL, ",");
    gps_lat = strtok(NULL, ",");

    gps_longitude = (float)atof(gps_lon);
    gps_latitude = (float)atof(gps_lat);
    
//    Serial.print("LAT: ");
//    Serial.println(gps_longitude);
    _data[11] = gps_longitude;
//    Serial.print("LON: ");
    _data[10] = gps_latitude;
//    Serial.println(gps_latitude);
}
}

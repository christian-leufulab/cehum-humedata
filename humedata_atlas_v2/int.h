void float2Bytes(float val,byte* bytes_array){
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  u.float_variable = val;
  memcpy(bytes_array, u.temp_array, 4);
}

void env_pressure(){
  ENV.begin();
  internal_pressure = ENV.readPressure();
  _data[7] = internal_pressure;
  internal_temperature = ENV.readTemperature();
  _data[12] = internal_temperature;
  internal_humidity = ENV.readHumidity();
  _data[13] = internal_humidity;
  ENV.end();
}

void read_battery_level(){
  batt_level = analogRead(A1);
  _data[14] = batt_level;
}

void write_to_sd(float data0, float data1,float data2,float data3,float data4,
                float data5, float data6, float data7, float data8, float data9,
                float data10, float data11, float data12, float data13, float data14,
                float data15, float data16, float data17, float data18, float data19,
                float data20, float data21) 
  {
  SPI.begin();
  delay(100);
  SD.begin(SD_CS);
  dataFile = SD.open("log-0000.csv", FILE_WRITE);
  delay(1000);
  
  dataFile.print(data0);
  dataFile.print(",");
  dataFile.print(data1);
  dataFile.print(",");
  dataFile.print(data2);
  dataFile.print(",");
  dataFile.print(data3);
  dataFile.print(",");
  dataFile.print(data4);
  dataFile.print(",");
  dataFile.print(data5);
  dataFile.print(",");
  dataFile.print(data6);
  dataFile.print(",");
  dataFile.print(data7);
  dataFile.print(",");
  dataFile.print(data8);
  dataFile.print(",");
  dataFile.print(data9);
  dataFile.print(",");
  dataFile.print(data10);
  dataFile.print(",");
  dataFile.print(data11);
  dataFile.print(",");
  dataFile.print(data12);
  dataFile.print(",");
  dataFile.print(data13);
  dataFile.print(",");
  dataFile.print(data14);
  dataFile.print(",");
  dataFile.print(data15);
  dataFile.print(",");
  dataFile.print(data16);
  dataFile.print(",");
  dataFile.print(data17);
  dataFile.print(",");
  dataFile.print(data18);
  dataFile.print(",");
  dataFile.print(data19);
  dataFile.print(",");
  dataFile.print(data20);
  dataFile.print(",");
  dataFile.println(data21);

  dataFile.close();
  delay(1000);
  SPI.end();
}

void generate_lorawan_array()
{
  float2Bytes(gps_latitude,&gps_latitude_float_bytes[0]);
  float2Bytes(gps_longitude,&gps_longitude_float_bytes[0]);
  float2Bytes(_data[2] /*EC*/,&ec_float_bytes[0]);
  float2Bytes(_data[3] /*TDS*/,&tds_float_bytes[0]);
  float2Bytes(_data[15] /*ORP*/,&orp_float_bytes[0]);
  float2Bytes(_data[0] /*DO*/,&do_float_bytes[0]);
  float2Bytes(_data[17] /*DOTEMP*/,&do_temp_float_bytes[0]);
  float2Bytes(_data[19] /*ECTEMP*/,&ec_temp_float_bytes[0]);  
  float2Bytes(_data[21] /*DO15*/,&do_15_float_bytes[0]);  

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
  
  _data_lorawan[29] = uint8_t  ((_data[14] - 2144.0) * 255.0/670.0);   // Battery Level 
  
  _data_lorawan[30] =   orp_float_bytes[0];                            // ORP
  _data_lorawan[31] =   orp_float_bytes[1];                            // ORP
  _data_lorawan[32] =   orp_float_bytes[2];                            // ORP
  _data_lorawan[33] =   orp_float_bytes[3];                            // ORP

  _data_lorawan[34] = uint8_t (_data[16] * 255/150.0);                   // SAT
  
  _data_lorawan[35] =   do_temp_float_bytes[0];                        // Temperature compensated DO
  _data_lorawan[36] =   do_temp_float_bytes[1];                        // Temperature compensated DO
  _data_lorawan[37] =   do_temp_float_bytes[2];                        // Temperature compensated DO
  _data_lorawan[38] =   do_temp_float_bytes[3];                        // Temperature compensated DO

  _data_lorawan[39] = uint8_t (_data[18] * 255/150.0);                   // Temperature compensated saturation 

  _data_lorawan[40] = ec_temp_float_bytes[0];                          // Temperature compensated EC
  _data_lorawan[41] = ec_temp_float_bytes[1];                          // Temperature compensated EC
  _data_lorawan[42] = ec_temp_float_bytes[2];                          // Temperature compensated EC
  _data_lorawan[43] = ec_temp_float_bytes[3];                          // Temperature compensated EC

  _data_lorawan[44] = uint8_t (_data[20] * 255/14.0);                  // Temperature compensated pH

  _data_lorawan[45] = do_15_float_bytes[0];
  _data_lorawan[46] = do_15_float_bytes[1];
  _data_lorawan[47] = do_15_float_bytes[2];
  _data_lorawan[48] = do_15_float_bytes[3];

  Serial.println("LORAWAN HEX DATA: ");
  
  for(int a = 0; a < sizeof(_data_lorawan); a++)
  {
    Serial.print("0x");
    Serial.print(_data_lorawan[a], HEX);
    Serial.print(" ");
  }

  Serial.println();
}

void send_lorawan_data()
{
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
  modem.write(_data_lorawan[34]);
  modem.write(_data_lorawan[35]);
  modem.write(_data_lorawan[36]);
  modem.write(_data_lorawan[37]);
  modem.write(_data_lorawan[38]);
  modem.write(_data_lorawan[39]);
  modem.write(_data_lorawan[40]);
  modem.write(_data_lorawan[41]);
  modem.write(_data_lorawan[42]);
  modem.write(_data_lorawan[43]);
  modem.write(_data_lorawan[44]);
  modem.write(_data_lorawan[45]);
  modem.write(_data_lorawan[46]);
  modem.write(_data_lorawan[47]);
  modem.write(_data_lorawan[48]);
  
  err = modem.endPacket(true);

  if (err > 0) {
    Serial.println("-- MENSAJE ENVIADO CORRECTAMENTE A TRAVÉS DE LORAWAN --");
  } else {
    Serial.println("-- ERROR ENVIANDO EL MENSAJE A TRAVÉS DE LORAWAN --");
  }
}

void print_data()
{
  Serial.println("FLOAT DATA:");
  Serial.print(" [");
  for (int i = 0; i < _data_size ; i++) {
    Serial.print(_data[i]);
    if (i < _data_size -1) {
      Serial.print(", ");
    }
  }
  Serial.println("]");
}

void sd_init()
{
  SPI.begin();
  delay(100);
  SD.begin(SD_CS);
  dataFile = SD.open("log-0000.csv", FILE_WRITE);
  delay(1000);
  dataFile.println("DissolvedOxygen,pH,ElectricalConductivity,TotalDissolvedSolids,Salinity,RelativeDensity,WaterTemperature,InternalPressure,AtmosphericPressure,AtmosphericTemperature,Latitude,Longitude,InternalTemperature,InternalHumidity,BatteryLevel,ORP,Saturation,DissolvedOxygenTemp,SaturationTemp,ElectricalConductivityTemp,pHTemp");
  dataFile.close();
  delay(100);
  SPI.end();
}

void lorawan_init()
{
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
}

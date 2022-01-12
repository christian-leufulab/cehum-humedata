void env_pressure(){
  ENV.begin();
  internal_pressure = ENV.readPressure();
//  Serial.print("-- INTERNAL PRESSURE: ");
  _data[7] = internal_pressure;
//  Serial.print(internal_pressure);
//  Serial.println(" KPA --");

  internal_temperature = ENV.readTemperature();
  _data[12] = internal_temperature;

  internal_humidity = ENV.readHumidity();
  _data[13] = internal_humidity;

  ENV.end();
}

void read_battery_level(){
  batt_level = analogRead(A1) * 0.25;
  _data[14] = batt_level;
}

void write_to_sd(float data0, float data1,float data2,float data3,float data4,
                float data5, float data6, float data7, float data8, float data9,
                float data10, float data11, float data12, float data13, float data14,
                float data15) 
  {
  SPI.begin();
  delay(100);
  SD.begin(sd_cs_pin);
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
  dataFile.println(data11);
  dataFile.print(",");
  dataFile.print(data12);
  dataFile.print(",");
  dataFile.println(data13);
  dataFile.print(",");
  dataFile.println(data14);
  dataFile.print(",");
  dataFile.println(data15);

  dataFile.close();
  delay(1000);
  SPI.end();
}

void get_time(){
  rtc.begin();
  DateTime now = rtc.now();
  Serial.print("-- ACTUAL TIME: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" AND DATE IS: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.println(" ");
  delay(1000);
}

void sleep_sensors(){
  Wire.begin();
  Wire.beginTransmission(rtd_address);
  Wire.write("sleep");
  Wire.endTransmission();

  Wire.begin();
  Wire.beginTransmission(ph_address);
  Wire.write("sleep");
  Wire.endTransmission();

  Wire.begin();
  Wire.beginTransmission(orp_address);
  Wire.write("sleep");
  Wire.endTransmission();

  Wire.begin();
  Wire.beginTransmission(do_address);
  Wire.write("sleep");
  Wire.endTransmission();

  Wire.begin();
  Wire.beginTransmission(ec_address);
  Wire.write("sleep");
  Wire.endTransmission();
}

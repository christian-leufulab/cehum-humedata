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
  batt_level = analogRead(A1);
  _data[14] = batt_level;
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

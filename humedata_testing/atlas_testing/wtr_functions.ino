void do_wire_transmission(){
  Wire.beginTransmission(do_address);
  Wire.write("r");
  Wire.endTransmission();
  delay(do_time);
  Wire.requestFrom(do_address, 20, 1);
  do_code = Wire.read();
  while(Wire.available()){
    do_in_char = Wire.read();
    do_data[do_i] = do_in_char;
    do_i++;
    if(do_in_char == 0){
      do_i = 0;
      break;
    }
  }
  do_i = 0;

  DO = strtok(do_data, ",");
  sat = strtok(NULL, ",");  

  /*
   * DEBUGGING
   */
//  Serial.print("-- DISSOLVED OXYGEN: ");
  _data[0] = (float)atof(DO);
  _data[16] = (float)atof(sat);
//  Serial.print(_data[0]);
//  Serial.println(" --");
  /*
   * DEBUGGING
   */
}

void do_temp_wire_transmission(){
  Wire.beginTransmission(do_address);
  Wire.write("t,");
  Wire.write(inst_temp.c_str());
  Wire.endTransmission();
  delay(300);
  Wire.beginTransmission(do_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(do_time);
  Wire.requestFrom(do_address, 20, 1);
  do_code = Wire.read();
  while(Wire.available()){
    do_in_char = Wire.read();
    do_data[do_i] = do_in_char;
    do_i++;
    if(do_in_char == 0){
      do_i = 0;
      break;
    }
  }
  do_i = 0;
  
  DO_temp = strtok(do_data, ",");
  sat_temp = strtok(NULL, ",");  

  /*
   * DEBUGGING
   */
//  Serial.print("-- DISSOLVED OXYGEN: ");
  _data[17] = (float)atof(DO_temp);
  _data[18] = (float)atof(sat_temp);
//  Serial.print(_data[0]);
//  Serial.println(" --");
  /*
   * DEBUGGING
   */
}

void do_15_wire_transmission(){
  Wire.beginTransmission(do_address);
  Wire.write("t,15.0");
  Wire.endTransmission();
  delay(300);
  Wire.beginTransmission(do_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(do_time);
  Wire.requestFrom(do_address, 20, 1);
  do_code = Wire.read();
  while(Wire.available()){
    do_in_char = Wire.read();
    do_data[do_i] = do_in_char;
    do_i++;
    if(do_in_char == 0){
      do_i = 0;
      break;
    }
  }
  do_i = 0;
  
  DO_15 = strtok(do_data, ",");
  sat_15 = strtok(NULL, ",");  

  /*
   * DEBUGGING
   */
//  Serial.print("-- DISSOLVED OXYGEN: ");
  _data[21] = (float)atof(DO_15);
//  Serial.print(_data[0]);
//  Serial.println(" --");
  /*
   * DEBUGGING
   */
}

void ph_wire_transmission(){
  Wire.beginTransmission(ph_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(ph_time);
  Wire.requestFrom(ph_address, 20, 1);
  ph_code = Wire.read();

  while(Wire.available()){
    ph_in_char = Wire.read();
    ph_data[ph_i] = ph_in_char;
    ph_i++;
    if(ph_in_char == 0){
      ph_i = 0;
      break;
    }
  }
  ph_i = 0;
//  Serial.print("-- PH: ");
  _data[1] = (float)atof(ph_data);
//  Serial.print(_data[1]);
//  Serial.println(" --");
}

void ph_temp_wire_transmission(){
  Wire.beginTransmission(ph_address);
  Wire.write("t,");
  Wire.write(inst_temp.c_str());
  Wire.endTransmission();
  delay(300);
  Wire.beginTransmission(ph_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(ph_time);
  Wire.requestFrom(ph_address, 20, 1);
  ph_code = Wire.read();

  while(Wire.available()){
    ph_in_char = Wire.read();
    ph_data_temp[ph_i] = ph_in_char;
    ph_i++;
    if(ph_in_char == 0){
      ph_i = 0;
      break;
    }
  }
  ph_i = 0;
//  Serial.print("-- PH: ");
  _data[20] = (float)atof(ph_data_temp);
//  Serial.print(_data[1]);
//  Serial.println(" --");
}

void ec_wire_transmission(){
  Wire.beginTransmission(ec_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(ec_time);
  Wire.requestFrom(ec_address, 32, 1);
  ec_code = Wire.read();
  while(Wire.available()){
    ec_in_char = Wire.read();
    ec_data[ec_i] = ec_in_char;
    ec_i++;
    if(ec_in_char == 0){
      ec_i = 0;
      break;
    }
  }
  ec_i = 0;
  ec = strtok(ec_data, ",");
  tds = strtok(NULL, ",");
  sal = strtok(NULL, ",");
  sg = strtok(NULL, ",");

//  Serial.print("-- ELECTRICAL CONDUCTIVITY ");
//  Serial.print("EC: ");
  _data[2] = (float)atof(ec);
//  Serial.print(_data[2]);
//  Serial.print(" || TDS: ");
  _data[3] = (float)atof(tds);
//  Serial.print(_data[3]);
//  Serial.print(" || SAL: ");
  _data[4] = (float)atof(sal);
//  Serial.print(_data[4]);
//  Serial.print(" || SG: ");
  _data[5] = (float)atof(sg);
//  Serial.print(_data[5]);
//  Serial.println(" --");
}

void ec_temp_wire_transmission(){
  Wire.beginTransmission(ec_address);
  Wire.write("t,");
  Wire.write(inst_temp.c_str());
  Wire.endTransmission();
  delay(300);
  Wire.beginTransmission(ec_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(ec_time);
  Wire.requestFrom(ec_address, 32, 1);
  ec_code = Wire.read();
  while(Wire.available()){
    ec_in_char = Wire.read();
    ec_data[ec_i] = ec_in_char;
    ec_i++;
    if(ec_in_char == 0){
      ec_i = 0;
      break;
    }
  }
  ec_i = 0;
  
  ec_temp = strtok(ec_data, ",");
  tds = strtok(NULL, ",");
  sal = strtok(NULL, ",");
  sg = strtok(NULL, ",");

  _data[19] = (float)atof(ec_temp);
}

void rtd_wire_transmission(){
  Wire.beginTransmission(rtd_address);
  Wire.write('r');
  Wire.endTransmission();
  delay(rtd_time_);
  Wire.requestFrom(rtd_address, 20, 1);
  rtd_code = Wire.read();
  while(Wire.available()){
    rtd_in_char = Wire.read();
    rtd_data[rtd_i] = rtd_in_char;
    rtd_i++;
    if(rtd_in_char == 0){
      rtd_i = 0;
      break;
    }
  }

  /*
   * DEBUGGING
   */
//  Serial.print("-- RTD: ");
  _data[6] = (float)atof(rtd_data);
//  Serial.print(_data[6]);
//  Serial.println(" --");
  /*
   * DEBUGGING
   */
}

void orp_wire_transmission(){
  Wire.beginTransmission(orp_address);
  Wire.write("r");
  Wire.endTransmission();
  delay(orp_time);
  Wire.requestFrom(orp_address, 20, 1);
  orp_code = Wire.read();
  while(Wire.available()){
    orp_in_char = Wire.read();
    orp_data[orp_i] = orp_in_char;
    orp_i++;
    if(orp_in_char == 0){
      orp_i = 0;
      break;
    }
  }
  _data[15] = (float)atof(orp_data);
}

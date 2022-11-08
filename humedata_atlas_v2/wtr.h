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

  _data[0] = (float)atof(DO);
  _data[16] = (float)atof(sat);
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

  _data[17] = (float)atof(DO_temp);
  _data[18] = (float)atof(sat_temp);
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

  _data[21] = (float)atof(DO_15);
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
  _data[1] = (float)atof(ph_data);
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
  _data[20] = (float)atof(ph_data_temp);
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

  _data[2] = (float)atof(ec);
  _data[3] = (float)atof(tds);
  _data[4] = (float)atof(sal);
  _data[5] = (float)atof(sg);
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

  _data[6] = (float)atof(rtd_data);
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

void read_atlas_sensors()
{
  digitalWrite(ATLAS_SWITCH, HIGH);
  delay(1000);
  Wire.begin();
  rtd_wire_transmission();
  inst_temp = String(_data[6]);  
  do_wire_transmission();
  ph_wire_transmission();
  ec_wire_transmission();
  orp_wire_transmission();
  do_temp_wire_transmission();
  ec_temp_wire_transmission();
  do_15_wire_transmission();
  ph_temp_wire_transmission();
}

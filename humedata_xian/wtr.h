/*
             MEASUREMENT UNITS      
 *[0]  --> DISSOLVED OXYGEN          [mg/L]
 *[1]  --> PH                        [-]
 *[2]  --> ELECTRICAL CONDUCTIVITY   [uS/cm]
 *[3]  --> TOTAL DISSOLVED SOLIDS    [ppm]
 *[4]  --> SALINITY                  [ppt]
 *[5]  --> RELATIVE DENSITY          [-]
 *[6]  --> WATER TEMPERATURE         [°C]
 *[7]  --> INTERNAL PRESSURE         [hPa]
 *[8]  --> ATMOSPHERIC PRESSURE      [hPa]
 *[9]  --> ATMOSPHERIC TEMPERATURE   [°C]
 *[10] --> GPS LATITUDE              [°]
 *[11] --> GPS LONGITUDE             [°]
 *[12] --> INTERNAL TEMPERATURE      [°C]
 *[13] --> INTERNAL HUMIDITY         [%]
 *[14] --> BATTERY LEVEL             [V]
 *[15] --> ORP                       [mV]
 *[16] --> SATURATION                [%]
 */

void read_xian_sensors()
{
  digitalWrite(gps_switch, LOW);
  digitalWrite(xian_switch, HIGH);
  delay(500);
  while((orp_received == 0) && (reading_tries < 5))
    {
      for(int i=0; i<8; i++)
      {
        Serial1.write(request_orp[i]);
      }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(orp_readings, 50);
          orp_f = bytes2Float(orp_readings[3], orp_readings[4], orp_readings[5], orp_readings[6]);
          orp_received = 1;
        }
        else
        {
          delay(500);
          orp_readings[3] = 0;
          orp_readings[4] = 0;
          orp_readings[5] = 0;
          orp_readings[6] = 0;
          orp_f = bytes2Float(orp_readings[3], orp_readings[4], orp_readings[5], orp_readings[6]);
          reading_tries++;
        }
    }
    
    reading_tries = 0;
    while((ec_received == 0) && (reading_tries < 5))
    {
      for(int i=0; i<8; i++)
      {
        Serial1.write(request_ec[i]);
      }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(ec_readings, 50);
          ec_f = bytes2Float(ec_readings[3], ec_readings[4], ec_readings[5], ec_readings[6]);
          ec_received = 1;
        }
        else
        {
          delay(500);
          ec_readings[3] = 0;
          ec_readings[4] = 0;
          ec_readings[5] = 0;
          ec_readings[6] = 0;
          ec_f = bytes2Float(ec_readings[3], ec_readings[4], ec_readings[5], ec_readings[6]);
          reading_tries++;
        }
    }
    
    reading_tries = 0;
    while((ph_received == 0) && (reading_tries < 5))
    {
      for(int i=0; i<8; i++)
      {
        Serial1.write(request_ph[i]);
      }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(ph_readings, 50);
          ph_f = bytes2Float(ph_readings[3], ph_readings[4], ph_readings[5], ph_readings[6]);
          ph_received = 1;
        }
        else
        {
          delay(500);
          ph_readings[3] = 0;
          ph_readings[4] = 0;
          ph_readings[5] = 0;
          ph_readings[6] = 0;
          ph_f = bytes2Float(ph_readings[3], ph_readings[4], ph_readings[5], ph_readings[6]);
          reading_tries++;
        }
    }
  
    while((do_received == 0) && (reading_tries < 5))
    {
      for(int i=0; i<8; i++)
      {
        Serial1.write(request_do[i]);
      }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(do_readings, 50);
          temp_f = bytes2Float(do_readings[3], do_readings[4], do_readings[5], do_readings[6]);
          sat_f = bytes2Float(do_readings[7], do_readings[8], do_readings[9], do_readings[10])*100;
          do_f = bytes2Float(do_readings[11], do_readings[12], do_readings[13], do_readings[14]);
          do_received = 1;
        }
        else
        {
          delay(500);
          do_readings[3] = 0;
          do_readings[4] = 0;
          do_readings[5] = 0;
          do_readings[6] = 0;
          do_readings[7] = 0;
          do_readings[8] = 0;
          do_readings[9] = 0;
          do_readings[10] = 0;
          do_readings[11] = 0;
          do_readings[12] = 0;
          do_readings[13] = 0;
          do_readings[14] = 0;
          temp_f = bytes2Float(do_readings[3], do_readings[4], do_readings[5], do_readings[6]);
          sat_f = bytes2Float(do_readings[7], do_readings[8], do_readings[9], do_readings[10])*100;
          do_f = bytes2Float(do_readings[11], do_readings[12], do_readings[13], do_readings[14]);
          reading_tries++;
        }
    }

    _data[0] = do_f;
    _data[1] = ph_f;
    _data[2] = ec_f;
    _data[3] = 0;
    _data[4] = 0;
    _data[5] = 0;
    _data[6] = temp_f;
    _data[15] = orp_f;
    _data[16] = sat_f;
      
    do_received  = 0;
    ph_received  = 0;
    ec_received  = 0;
    orp_received = 0;
    reading_tries = 0;
    digitalWrite(xian_switch, LOW);
    delay(500);
}

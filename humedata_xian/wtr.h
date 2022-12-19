void read_xian_sensors()
{
  // Se apaga el módulo GPS para que no interfiera con la lectura de los sensores Xi'An
  digitalWrite(GPS_SWITCH, LOW);
  // Se encienden los sensores Xi'An y también el módulo RS-485
  digitalWrite(XIAN_SWITCH, HIGH);
  digitalWrite(RS485_SWITCH, HIGH);
  delay(500);
  // Se intenta un máximo de 5 veces la lecura de los sensores Xi'An
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
          if(orp_f != 0.0)
          {
            orp_received = 1;
          }
        }
        else
        {
          delay(1000);
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
          delay(1000);
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
          delay(1000);
          ph_readings[3] = 0;
          ph_readings[4] = 0;
          ph_readings[5] = 0;
          ph_readings[6] = 0;
          ph_f = bytes2Float(ph_readings[3], ph_readings[4], ph_readings[5], ph_readings[6]);
          reading_tries++;
        }
    }
    
    reading_tries = 0;
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
          delay(1000);
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

    // Se apagan los sensores Xi'An y el módulo RS-485
    digitalWrite(XIAN_SWITCH, LOW);
    digitalWrite(RS485_SWITCH, LOW);
}

const int RS485_SWITCH = A6;
const int XIAN_SWITCH = A4;

byte request_ph[]   = {0x01, 0x03, 0x28, 0x00, 0x00, 0x02, 0xcd, 0xab};
byte cal_ph4[]      = {0x01, 0x10, 0x23, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x80, 0x40, 0x1e, 0xae};
byte cal_ph7[]      = {0x01, 0x10, 0x23, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0xe0, 0x40, 0x36, 0xae};
byte cal_ph10[]     = {0x01, 0x10, 0x23, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x20, 0x41, 0xa7, 0x6e};
byte id[]           = {0xff, 0x03, 0x30, 0x00, 0x00, 0x01, 0x9e, 0xd4};
byte cal_param[]    = {0x01, 0x03, 0x11, 0x00, 0x00, 0x04, 0x41, 0x35};
byte ph_id[50];
byte ph_readings[50];
byte ph_cal_response[50];
byte cal_params[50];

float k = 0.0;
float b = 0.0;
float ph_f   = 0.0;

String incomingString;
int reading_tries = 0;
int ph_received = 0;

float bytes2Float(byte byte_0, byte byte_1, byte byte_2, byte byte_3)
{
  long x = (long)byte_3<<24|(long)byte_2<<16|byte_1<<8|byte_0;
  union
  {
    long y;
    float z;
  }data;
  data.y = x;
  return data.z;
}

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  Serial1.begin(9600);

  pinMode(XIAN_SWITCH, OUTPUT);
  pinMode(RS485_SWITCH, OUTPUT);
  digitalWrite(XIAN_SWITCH, HIGH);
  digitalWrite(RS485_SWITCH, HIGH);

  Serial.println();
  Serial.println("BIENVENIDO AL ASISTENTE DE CALIBRACIÓN DEL SENSOR DE PH XI'AN");
  Serial.println();
  Serial.println("PARA OBTENER LOS COMANDOS DISPONIBLES ESCRIBA <<AYUDA>> EN EL TERMINAL");
  Serial.println();
  Serial.println("PARA CALIBRAR, INSERTE LA SONDA EN LA SOLUCIÓN ADECUADA Y REALICE VARIAS LECTURAS");
  Serial.println("HASTA QUE EL VALOR SE ESTABILICE. UNA VEZ EL VALOR ESTÉ ESTABILIZADO, EFECTÚE LA ");
  Serial.println("CALIBRACIÓN COMENZANDO POR PH 4, LUEGO PH 7 Y FINALMENTE PH 10");
  Serial.println();
}

void loop() 
{
  if (Serial.available() > 0) {
    incomingString = Serial.readStringUntil('\n');

    if(incomingString == "CAL7\r")
    {
      while((ph_received == 0) && (reading_tries < 5))
      {
        for(int i=0; i<13 ; i++)
        {
          Serial1.write(cal_ph7[i]);
        }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(ph_cal_response, 50);
          Serial.println();
          Serial.println("LA RESPUESTA DEL SENSOR ES: ");
          Serial.println(ph_cal_response[0], HEX);
          Serial.println(ph_cal_response[1], HEX);
          Serial.println(ph_cal_response[2], HEX);
          Serial.println(ph_cal_response[3], HEX);
          Serial.println(ph_cal_response[4], HEX);
          Serial.println(ph_cal_response[5], HEX);
          Serial.println(ph_cal_response[6], HEX);
          Serial.println(ph_cal_response[7], HEX);
          Serial.println();
          ph_received = 1;
        }
        else
        {
          Serial.println();
          reading_tries++;
          if(reading_tries == 5)
          {
            Serial.println("NO SE HA PODIDO ESTABLECER COMUNICACIÓN CON EL SENSOR");
          }
          Serial.println();
          delay(500);
        }
      }
      reading_tries = 0;
      ph_received = 0;
    }
    else if(incomingString == "CAL4\r")
    {
      while((ph_received == 0) && (reading_tries < 5))
      {
        for(int i=0; i<13 ; i++)
        {
          Serial1.write(cal_ph4[i]);
        }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(ph_cal_response, 50);
          Serial.println();
          Serial.println("LA RESPUESTA DEL SENSOR ES: ");
          Serial.println(ph_cal_response[0], HEX);
          Serial.println(ph_cal_response[1], HEX);
          Serial.println(ph_cal_response[2], HEX);
          Serial.println(ph_cal_response[3], HEX);
          Serial.println(ph_cal_response[4], HEX);
          Serial.println(ph_cal_response[5], HEX);
          Serial.println(ph_cal_response[6], HEX);
          Serial.println(ph_cal_response[7], HEX);
          Serial.println();
          ph_received = 1;
        }
        else
        {
          Serial.println();
          reading_tries++;
          if(reading_tries == 5)
          {
            Serial.println("NO SE HA PODIDO ESTABLECER COMUNICACIÓN CON EL SENSOR");
          }
          Serial.println();
          delay(500);
        }
      }
      reading_tries = 0;
      ph_received = 0;
    }
    else if(incomingString == "CAL10\r")
    {
      while((ph_received == 0) && (reading_tries < 5))
      {
        for(int i=0; i<13 ; i++)
        {
          Serial1.write(cal_ph10[i]);
        }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(ph_cal_response, 50);
          Serial.println();
          Serial.println("LA RESPUESTA DEL SENSOR ES: ");
          Serial.println(ph_cal_response[0], HEX);
          Serial.println(ph_cal_response[1], HEX);
          Serial.println(ph_cal_response[2], HEX);
          Serial.println(ph_cal_response[3], HEX);
          Serial.println(ph_cal_response[4], HEX);
          Serial.println(ph_cal_response[5], HEX);
          Serial.println(ph_cal_response[6], HEX);
          Serial.println(ph_cal_response[7], HEX);
          Serial.println();
          ph_received = 1;
        }
        else
        {
          Serial.println();
          reading_tries++;
          if(reading_tries == 5)
          {
            Serial.println("NO SE HA PODIDO ESTABLECER COMUNICACIÓN CON EL SENSOR");
          }
          Serial.println();
          delay(500);
        }
      }
      reading_tries = 0;
      ph_received = 0;
    }
    else if((incomingString == "AYUDA\r") || (incomingString == "ayuda\r"))
    {
      Serial.println("LA LISTA DE COMANDOS ES LA SIGUIENTE: ");
      Serial.println();
      Serial.println("AYUDA:   DESPLIEGA EL MENÚ DE AYUDA CON LOS COMANDOS DISPONIBLES");
      Serial.println("L:       SOLICITA UNA LECTURA AL SENSOR");
      Serial.println("CAL4:    REALIZA LA CALIBRACIÓN AL SENSOR EN EL PUNTO DE PH 4");
      Serial.println("CAL7:    REALIZA LA CALIBRACIÓN AL SENSOR EN EL PUNTO DE PH 7");
      Serial.println("CAL10:   REALIZA LA CALIBRACIÓN AL SENSOR EN EL PUNTO DE PH 10");
      Serial.println("ID:      CONSULTA POR LA DIRECCIÓN DE IDENTIFICACIÓN DEL SENSOR");
      Serial.println("PARAM:   ENVÍA LOS PARÁMETROS DE CALIBRACIÓN <<K>> Y <<B>>");
      Serial.println();
      Serial.println("PARA CALIBRAR, INSERTE LA SONDA EN LA SOLUCIÓN ADECUADA Y REALICE VARIAS LECTURAS");
      Serial.println("HASTA QUE EL VALOR SE ESTABILICE. UNA VEZ EL VALOR ESTÉ ESTABILIZADO, EFECTÚE LA ");
      Serial.println("CALIBRACIÓN COMENZANDO POR PH 4, LUEGO PH 7 Y FINALMENTE PH 10");
      Serial.println();
    }
    else if((incomingString == "L\r") || (incomingString == "l\r"))
    {
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
          Serial.println();
          Serial.println("EL VALOR DE PH DE LA SONDA ES DE: ");
          Serial.println(ph_f);
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
          if(reading_tries > 4)
          {
            Serial.println();
            Serial.println("SE HA EXCEDIDO EL NÚMERO DE INTENTOS DE COMUNICACIÓN CON LA SONDA");
            Serial.println("POR FAVOR CHEQUEE QUE LA SONDA ESTÉ CONECTADA, ALIMENTADA Y QUE LA");
            Serial.println("DIRECCIÓN DE COMUNICACIÓN SEA LA CORRECTA");
            Serial.println();
          }
        }
    } 
    reading_tries = 0;
    ph_received = 0;
    }
    else if((incomingString == "param\r") || (incomingString == "PARAM\r"))
    {
      while((ph_received == 0) && (reading_tries < 5))
      {
        for(int i=0; i<8 ; i++)
        {
          Serial1.write(cal_param[i]);
        }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(cal_params, 50);
          Serial.println();
          Serial.println("LA RESPUESTA DEL SENSOR ES: ");
          Serial.println(cal_params[0], HEX);
          Serial.println(cal_params[1], HEX);
          Serial.println(cal_params[2], HEX);
          Serial.println(cal_params[3], HEX);
          Serial.println(cal_params[4], HEX);
          Serial.println(cal_params[5], HEX);
          Serial.println(cal_params[6], HEX);
          Serial.println(cal_params[7], HEX);
          Serial.println(cal_params[8], HEX);
          Serial.println(cal_params[9], HEX);
          Serial.println(cal_params[10], HEX);
          Serial.println(cal_params[11], HEX);
          Serial.println(cal_params[12], HEX);
          Serial.println();
          
          Serial.print("LOS PARÁMETROS DE CALIBRACIÓN SON K: ");
          k = bytes2Float(cal_params[3], cal_params[4], cal_params[5], cal_params[6]);
          Serial.print(k);
          Serial.print(" Y B: ");
          b = bytes2Float(cal_params[7], cal_params[8], cal_params[9], cal_params[10]);
          Serial.println(b);
          Serial.println();
          ph_received = 1;
        }
        else
        {
          Serial.println();
          reading_tries++;
          if(reading_tries == 5)
          {
            Serial.println("NO SE HA PODIDO ESTABLECER COMUNICACIÓN CON EL SENSOR");
          }
          Serial.println();
          delay(500);
        }
      }
      reading_tries = 0;
      ph_received = 0;
    }
    
    else if((incomingString == "ID\r") || (incomingString == "id\r"))
    {
      while((ph_received == 0) && (reading_tries < 5))
      {
        for(int i=0; i<8 ; i++)
        {
          Serial1.write(id[i]);
        }
        if(Serial1.available() > 0)
        {
          Serial1.readBytes(ph_id, 50);
          Serial.println();
          Serial.println("LA RESPUESTA DEL SENSOR ES: ");
          Serial.println(ph_id[0], HEX);
          Serial.println(ph_id[1], HEX);
          Serial.println(ph_id[2], HEX);
          Serial.println(ph_id[3], HEX);
          Serial.println(ph_id[4], HEX);
          Serial.println(ph_id[5], HEX);
          Serial.println(ph_id[6], HEX);
          Serial.println();
          Serial.print("LA DIRECCIÓN DEL SENSOR ES: ");
          Serial.println(ph_id[3], HEX);
          
          Serial.println();
          ph_received = 1;
        }
        else
        {
          Serial.println();
          reading_tries++;
          if(reading_tries == 5)
          {
            Serial.println("NO SE HA PODIDO ESTABLECER COMUNICACIÓN CON EL SENSOR");
          }
          Serial.println();
          delay(500);
        }
      }
      reading_tries = 0;
      ph_received = 0;
    }
  }
  
}

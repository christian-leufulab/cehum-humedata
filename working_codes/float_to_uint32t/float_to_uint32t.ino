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

float float_example = 25.33333;
byte bytes[4];


void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  float2Bytes(float_example,&bytes[0]);
  
  for(int i=3; i>-1; i--)
  {
    Serial.print("0x");
    Serial.print(bytes[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  delay(1000);

}

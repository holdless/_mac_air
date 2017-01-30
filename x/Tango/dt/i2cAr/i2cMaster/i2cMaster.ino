#include <Wire.h>

void setup()
{
  Wire.begin();
}

void loop()
{
  Wire.beginTransmission(3);
  Wire.write("hello\n");
  Wire.endTransmission();
  
  delay(1000);
}

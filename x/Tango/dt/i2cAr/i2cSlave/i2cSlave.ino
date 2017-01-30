#include <Wire.h>

void setup()
{
  Wire.begin(3);
  Wire.onReceive(receiveEvent); // receiveEvent 為自訂函數
  Serial.begin(9600);
}

void loop()
{
  delay(100); //從端接收的頻率應要比主端發射的頻率高
}

void receiveEvent(int numBytes) //numBytes為必要之引數，即使沒用到也要寫出來
{
  while (Wire.available())
  {
    char c = Wire.read();
    Serial.print(c);
  }
}


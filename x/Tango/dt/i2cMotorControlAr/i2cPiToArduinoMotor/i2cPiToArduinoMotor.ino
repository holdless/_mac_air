#include <Wire.h>

#define SLAVE_ADDRESS 0x04

//int state = 0;

////////////////////////
const byte EA = 6;
const byte EB = 5;

const byte IA = 4;
const byte IB = 7;
const byte IC = 8;
const byte ID = 9;

byte dir = 0;

void hold()
{
  analogWrite(EA, 0);
  analogWrite(EB, 0);
}

void forward(byte s)
{
  analogWrite(EA, s);
  analogWrite(EB, s);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  digitalWrite(IC, LOW);
  digitalWrite(ID, HIGH);
}

void backward(byte s)
{
  analogWrite(EA, s);
  analogWrite(EB, s);
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, LOW);
}

void turn_left(byte s)
{
  analogWrite(EA, s);
  analogWrite(EB, s);
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, LOW);
  digitalWrite(ID, HIGH);
}

void turn_right(byte s)
{
  analogWrite(EA, s);
  analogWrite(EB, s);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, LOW);
}

void forward_bias(byte l, byte r)
{
  analogWrite(EA, l);
  analogWrite(EB, r);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  digitalWrite(IC, LOW);
  digitalWrite(ID, HIGH);
}

void backward_bias(byte l, byte r)
{
  analogWrite(EA, l);
  analogWrite(EB, r);
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, LOW);
}

////
////////////////////

void setup() 
{
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
 
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}
 
void loop() 
{
  delay(100);
}
 
// callback for received data
void receiveData(int byteCount)
{ 
  ///// motor setting
  byte speed = 200;
  
  while(Wire.available()) 
  {
    dir = Wire.read();
 
    if (dir == 1)
      forward(speed);
    else if (dir == 2)
      backward(speed);
    else if (dir == 3)
      turn_left(speed - 50);
    else if (dir == 4)
      turn_right(speed - 50);
    else if (dir == 5)
      forward_bias(speed - 100, speed);
    else if (dir == 6)
      forward_bias(speed, speed - 100);
    else if (dir == 7)
      backward_bias(speed - 100, speed);
    else if (dir == 8)
      backward_bias(speed, speed - 100);
    else
      hold();
  }
}
 
// callback for sending data
void sendData()
{
  Wire.write(dir);
}

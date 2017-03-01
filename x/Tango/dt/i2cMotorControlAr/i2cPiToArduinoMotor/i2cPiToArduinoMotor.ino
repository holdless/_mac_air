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
  digitalWrite(IA, LOW);
  digitalWrite(IB, LOW);
  digitalWrite(IC, LOW);
  digitalWrite(ID, LOW);
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

void ccw(byte s)
{
  analogWrite(EA, s);
  analogWrite(EB, s);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, LOW);
}

void cw(byte s)
{
  analogWrite(EA, s);
  analogWrite(EB, s);
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, LOW);
  digitalWrite(ID, HIGH);
}

void up_left(byte s)
{
  analogWrite(EA, s);
//  analogWrite(EB, 0);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, HIGH);
}

void up_right(byte s)
{
//  analogWrite(EA, 0);
  analogWrite(EB, s);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, LOW);
  digitalWrite(ID, HIGH);
}

void down_left(byte s)
{
  analogWrite(EA, s);
//  analogWrite(EB, 0);
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, HIGH);
}

void down_right(byte s)
{
//  analogWrite(EA, 0);
  analogWrite(EB, s);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, LOW);
}

////////////////////

void setup() 
{
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
 
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  pinMode(EA,OUTPUT);//output
 pinMode(EB,OUTPUT);
 pinMode(IA,OUTPUT);
 pinMode(IB,OUTPUT);
 pinMode(IC,OUTPUT);
 pinMode(ID,OUTPUT);
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
 
    if (dir == 1) // up
      forward(speed);
    else if (dir == 2) // down
      backward(speed);
    else if (dir == 3) // left
      ccw(speed);
    else if (dir == 4) // right
      cw(speed);
    else if (dir == 5) // up-left
      up_left(speed);
    else if (dir == 6) // up-right
      up_right(speed);
    else if (dir == 7) // down-left
      down_left(speed);
    else if (dir == 8) // down-right
      down_right(speed);
    else
      hold();
  }
}
 
// callback for sending data
void sendData()
{
  Wire.write(dir);
}

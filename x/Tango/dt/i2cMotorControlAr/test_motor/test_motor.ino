const byte EA = 6;
const byte EB = 5;

const byte IA = 4;
const byte IB = 7;
const byte IC = 8;
const byte ID = 9;

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

void left(byte s)
{
  analogWrite(EA, s);
  analogWrite(EB, s);
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, LOW);
  digitalWrite(ID, HIGH);
}

void right(byte s)
{
  analogWrite(EA, s);
  analogWrite(EB, s);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, LOW);
}

////////////////
void setup() {
  pinMode(EA,OUTPUT);//output
  pinMode(EB,OUTPUT);
  pinMode(IA,OUTPUT);
  pinMode(IB,OUTPUT);
  pinMode(IC,OUTPUT);
  pinMode(ID,OUTPUT);

  byte lspeed = 255;
  byte rspeed = 255;
  forward(lspeed);
  delay(3000);
  hold();
  delay(1000);

  backward(lspeed);
  delay(3000);
  hold();
  delay(1000);

  left(lspeed);
  delay(3000);
  hold();
  delay(1000);

  right(lspeed);
  delay(3000);
  hold();
  delay(1000);
}
 
void loop() 
{
}
 

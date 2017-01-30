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

void setup() {
  byte lspeed = 200;
  byte rspeed = 200;
  forward(lspeed);
  delay(3000);
  hold();
  delay(1000);

  backward(lspeed);
  delay(3000);
  hold();
  delay(1000);

  turn_left(lspeed);
  delay(3000);
  hold();
  delay(1000);

  forward_bias(lspeed - 100, lspeed);
  delay(3000);
  hold();
  delay(1000);

  turn_right(lspeed);
  delay(3000);
  hold();
  delay(1000);

  forward_bias(lspeed, lspeed - 100);
  delay(3000);
  hold();
  delay(1000);

}
 
void loop() 
{
}
 

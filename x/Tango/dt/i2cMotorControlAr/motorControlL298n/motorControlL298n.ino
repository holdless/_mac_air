const byte speed = 200;

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

void forward()
{
  analogWrite(EA, speed);
  analogWrite(EB, speed);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, LOW);
}

void backward()
{
  analogWrite(EA, speed);
  analogWrite(EB, speed);
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, LOW);
  digitalWrite(ID, HIGH);
}

void turnRight()
{
  analogWrite(EA, speed);
  analogWrite(EB, speed);
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  digitalWrite(IC, HIGH);
  digitalWrite(ID, LOW);
}

void turnLeft()
{
  analogWrite(EA, speed);
  analogWrite(EB, speed);
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  digitalWrite(IC, LOW);
  digitalWrite(ID, HIGH);
}

void setup()
{
  delay(300);
  forward();
  delay(2000);
  hold();
  delay(300);
  backward();
  delay(2000);
  hold();
  delay(300);
  turnLeft();
  delay(1000);
  hold();
  delay(300);
  turnRight();
  delay(1000);
  hold();
}

void loop()
{
}


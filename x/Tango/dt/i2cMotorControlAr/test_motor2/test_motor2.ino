const byte ENA = 6;
const byte ENB = 5;

const byte IN1 = 4;
const byte IN2 = 7;
const byte IN3 = 8;
const byte IN4 = 9;

void setup()
{
 pinMode(ENA,OUTPUT);//output
 pinMode(ENB,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT);
 pinMode(IN3,OUTPUT);
 pinMode(IN4,OUTPUT);
 digitalWrite(ENA,LOW);
 digitalWrite(ENB,LOW);//stop driving
 digitalWrite(IN1,LOW); 
 digitalWrite(IN2,HIGH);//setting motorA's directon
 digitalWrite(IN3,HIGH);
 digitalWrite(IN4,LOW);//setting motorB's directon



  analogWrite(ENA,200);//start driving motorB
  analogWrite(ENB,100);//start driving motorB

}
void loop()
{

} 
